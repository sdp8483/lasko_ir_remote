/* Includes ------------------------------------------------------------------ */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/delay.h>
#include <stdint.h>

#include "clock.h"                                  /* set main clock based on platformio.ini f_cpu value */

/* Define -------------------------------------------------------------------- */
#define IR_CARRIER_FREQUENCY_HZ     38000   /* Hz, carrier frequency of IR signal */
#define IR_CARRIER_PERIOD_US        (1000000 / IR_CARRIER_FREQUENCY_HZ)
#define IR_BIT_DURATION_US          1680    /* useconds, spacing between bits */
#define IR_ONE_BIT_DURATION_US      1246    /* useconds, how long a '1' bit signal is on */
#define IR_ZERO_BIT_DURATION_US     404     /* useconds, how long a '0' bit signal is on */

#define IR_BIT_PULSES               int((IR_BIT_DURATION_US / IR_CARRIER_PERIOD_US) + 0.5) * 2
#define IR_ONE_BIT_PULSES           int((IR_ONE_BIT_DURATION_US / IR_CARRIER_PERIOD_US) + 0.5) * 2
#define IR_ONE_BIT_END_PULSES       (IR_BIT_PULSES - IR_ONE_BIT_PULSES)
#define IR_ZERO_BIT_PULSES          int((IR_ZERO_BIT_DURATION_US / IR_CARRIER_PERIOD_US) + 0.5) * 2
#define IR_ZERO_BIT_END_PULSES      (IR_BIT_PULSES - IR_ZERO_BIT_PULSES)

#define IR_REPEATS                  4       /* number of repeat transmissions */
#define IR_DELAY_BETWEEN_REPEATS_US 7107

#define TCA0_COUNTER_VALUE          int(((F_CPU / IR_CARRIER_FREQUENCY_HZ) / 2) + 0.5)  /* used to generate 38kHz, timer counts to this value */

#define BUTTON_DEBOUNCE_ms          50              /* minimum button delay when held down */

#define BUTTON_0_PIN                (1 << PIN2_bp)  /* button_0 net label from schematic */
#define BUTTON_1_PIN                (1 << PIN3_bp)  /* button_1 net label from schematic */
#define BUTTON_2_PIN                (1 << PIN6_bp)  /* button_2 net label from schematic */
#define BUTTON_3_PIN                (1 << PIN7_bp)  /* button_3 net label from schematic */

#define POWER_BUTTON                BUTTON_3_PIN    /* send power command to cycle though fan speeds */
#define OSCILLATE_BUTTON            BUTTON_2_PIN    /* send oscillate command to turn fuction on/off */
#define TIMER_BUTTON                BUTTON_1_PIN    /* send timer command to cycle through timer */
#define TEMPERATURE_BUTTON          BUTTON_0_PIN    /* send temperature command to cycle through temperatures */

#define POWER_COMMAND               0b110110000001  /* power command signal */
#define POWER_COMMAND_LEN           12              /* number of bits in power command signal */
#define OSCILLATE_COMMAND           0b110110000010  /* oscillate command signal */
#define OSCILLATE_COMMAND_LEN       12              /* number of bits in oscillate command signal */
#define TIMER_COMMAND               0b110110010000  /* timer command signal */
#define TIMER_COMMAND_LEN           12              /* number of bits in timer command signal */
#define TEMPERATURE_COMMAND         0b110110001000  /* temperature command signal */
#define TEMPERATURE_COMMAND_LEN     12              /* number of bits in temperature command signal */

/* Macro --------------------------------------------------------------------- */

/* Typedef ------------------------------------------------------------------- */
typedef enum buttons_t {
    POWER       = POWER_BUTTON,
    OSCILLATE   = OSCILLATE_BUTTON,
    TIMER       = TIMER_BUTTON,
    TEMPERATURE = TEMPERATURE_BUTTON,
    NONE        = 0
};

/* Globals ------------------------------------------------------------------- */
volatile buttons_t wake_button;  /* flag that is set on button ISR to signal to main code what button was pressed */

/* Function Prototypes ------------------------------------------------------- */
void ir_one_bit(void);
void ir_zero_bit(void);
void ir_sleep(void);
void ir_send(uint16_t cmd, int8_t len);
void button_wait_for_release(uint8_t button);

/* Main ---------------------------------------------------------------------- */
int main(void) {
    clock_init();                                   /* Set main clock frequency to value set in platformio.ini */

    /* setup buttons as inputs */
    PORTA.DIRCLR = (POWER_BUTTON | OSCILLATE_BUTTON | TIMER_BUTTON | TEMPERATURE_BUTTON);

    /* setup button interrupts on falling edge, enable pullup */
    PORTA.PIN2CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
    PORTA.PIN3CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
    PORTA.PIN6CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
    PORTA.PIN7CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));

    /* setup IR pin as out output*/
    PORTA.DIRSET = 1 << PIN1_bp;    /* set PA1 as output, WO1 from TCA0 */
    // PORTA.PIN1CTRL |= 1 << PORT_INVEN_bp;    /* invert PA1 output */

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        power_all_disable();    /* power off peripherals */
        sei();                  /* enable global interrupts */
        sleep_mode();           /* enable and sleep, wake here */
        power_all_enable();     /* power on peripherals */

        /* disable button interrupts */
        PORTA.PIN2CTRL |= PORT_ISC_INTDISABLE_gc;
        PORTA.PIN3CTRL |= PORT_ISC_INTDISABLE_gc;
        PORTA.PIN6CTRL |= PORT_ISC_INTDISABLE_gc;
        PORTA.PIN7CTRL |= PORT_ISC_INTDISABLE_gc;

        /* setup timer A */
        TCA0.SINGLE.CTRLESET = TCA_SINGLE_CMD_RESET_gc;
        TCA0.SINGLE.CMP0 = TCA0_COUNTER_VALUE;
        TCA0.SINGLE.CTRLB = (1 << TCA_SINGLE_CMP1EN_bp)     /* enable compare 1, PA1 */
                            | TCA_SINGLE_WGMODE_FRQ_gc;     /* waveform mode, frequency */
        TCA0.SINGLE.INTCTRL = 1 << TCA_SINGLE_OVF_bp;      /* enable ISR when counter reaches TOP (CMP0 in waveform frequency mode) */
        TCA0.SINGLE.CTRLA = (TCA_SINGLE_CLKSEL_DIV1_gc      /* system clock */
                            | (1 << TCA_SINGLE_ENABLE_bp)); /* enable */

        switch (wake_button) {
            case POWER:
                ir_send(POWER_COMMAND, POWER_COMMAND_LEN);
                button_wait_for_release(POWER_BUTTON);
                break;
            case OSCILLATE:
                ir_send(OSCILLATE_COMMAND, OSCILLATE_COMMAND_LEN);
                button_wait_for_release(OSCILLATE_BUTTON);
                break;
            case TIMER:
                ir_send(TIMER_COMMAND, TIMER_COMMAND_LEN);
                button_wait_for_release(TIMER_BUTTON);
                break;
            case TEMPERATURE:
                ir_send(TEMPERATURE_COMMAND, TEMPERATURE_COMMAND_LEN);
                button_wait_for_release(TEMPERATURE_BUTTON);
                break;
            default:
                break;
        }

        /* disable timer A */
        TCA0.SINGLE.CTRLA = (0 << TCA_SINGLE_ENABLE_bp);

        /* enable button interrupts */
        PORTA.PIN2CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
        PORTA.PIN3CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
        PORTA.PIN6CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
        PORTA.PIN7CTRL |= (PORT_ISC_FALLING_gc | (1 << PORT_PULLUPEN_bp));
    }
}

ISR(PORTA_PORT_vect) {
    wake_button = (buttons_t) VPORTA.INTFLAGS;

    VPORTA.INTFLAGS |= 0;   /* clear interrupts */
}

ISR(TCA0_OVF_vect) {
	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void ir_sleep(void) {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();          /* enable global interrupts */
    sleep_mode();   /* enables sleep, sleeps cpu, wakes from sleep and disables */
}

void ir_one_bit(void) {
    PORTA.DIRSET = 1 << PIN1_bp;    /* set PA1 as output */
    for (uint8_t i=0; i< IR_ONE_BIT_PULSES; i++) {
        ir_sleep();
    }

    PORTA.OUTCLR = 1 << PIN1_bp;
    PORTA.DIRCLR = 1 << PIN1_bp;    /* disable PA1 output */

    for (uint8_t i=0; i< IR_ONE_BIT_END_PULSES; i++) {
        ir_sleep();
    }
}

void ir_zero_bit(void) {
    PORTA.DIRSET = 1 << PIN1_bp;    /* set PA1 as output */
    for (uint8_t i=0; i< IR_ZERO_BIT_PULSES; i++) {
        ir_sleep();
    }

    PORTA.OUTCLR = 1 << PIN1_bp;
    PORTA.DIRCLR = 1 << PIN1_bp;    /* disable PA1 output */

    for (uint8_t i=0; i< IR_ZERO_BIT_END_PULSES; i++) {
        ir_sleep();
    }
}

void ir_send(uint16_t cmd, int8_t len) {
    for (uint8_t j=0; j<IR_REPEATS; j++) {
        for (int8_t i=len; i>=0; i--) {
            if ((cmd >> i) & 1) {
                ir_one_bit();
            } else {
                ir_zero_bit();
            }
        }

        _delay_us(IR_DELAY_BETWEEN_REPEATS_US);
    }
}

void button_wait_for_release(uint8_t button) {
    while (!(PORTA.IN & button)) {
        _delay_ms(BUTTON_DEBOUNCE_ms);
    }
}