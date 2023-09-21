#ifndef INCLUDE_USART_H
#define INCLUDE_USART_H

/* Includes ------------------------------------------------------------------ */
#include <avr/io.h>
#include <avr/delay.h>
#include <stdint.h>
#include <stdlib.h>

/* Defines ------------------------------------------------------------------- */
#define HEX     16
#define DEC     10
#define OCT     8
#define BIN     2

#define PORT_RXTX   PORTB
#define TX_PIN      PIN2_bm;
#define RX_PIN      PIN3_bm;

#define USART0_BAUD_RATE(BAUD_RATE)     ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

/* Macros -------------------------------------------------------------------- */
#define PRINT_EOL   print("\r\n")

/* typedef ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------- */

/* External Globals ---------------------------------------------------------- */

/* Function Prototypes ------------------------------------------------------- */

/* Class --------------------------------------------------------------------- */

class USART {
    public:
        void begin(uint16_t baud_rate);
        void end(void);

        void print(char *msg);
        void println(char *msg);

        void println(void);

        void print(int8_t value);
        void print(int8_t value, int8_t idx);
        void println(int8_t value);
        void println(int8_t value, int8_t idx);

        void print(uint8_t value);
        void print(uint8_t value, int8_t idx);
        void println(uint8_t value);
        void println(uint8_t value, int8_t idx);

        void print(int16_t value);
        void print(int16_t value, int8_t idx);
        void println(int16_t value);
        void println(int16_t value, int8_t idx);

        void print(uint16_t value);
        void print(uint16_t value, int8_t idx);
        void println(uint16_t value);
        void println(uint16_t value, int8_t idx);

        void print(int32_t value);
        void print(int32_t value, int8_t idx);
        void println(int32_t value);
        void println(int32_t value, int8_t idx);

        void print(uint32_t value);
        void print(uint32_t value, int8_t idx);
        void println(uint32_t value);
        void println(uint32_t value, int8_t idx);

    private:
        void _print_char(char c);
};

static USART Serial;

#endif