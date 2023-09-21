#include "usart.h"

void USART::begin(uint16_t baud_rate) {
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(baud_rate);

    PORT_RXTX.DIRSET = TX_PIN;  /* set Tx pin as output */
    PORT_RXTX.DIRCLR = RX_PIN;  /* set Rx pin as input */

    USART0.CTRLB |= (USART_TXEN_bm | USART_RXEN_bm);
}

void USART::end(void) {
    /* wait for Tx data to flush */
    _delay_ms(10);

    /* USART silicon errata
     * TX pin override not released when disabling transmitter 
     * workaround is to make sure receiver is enabled while disabling transmitter */
    USART0.CTRLB |= USART_RXEN_bm;
    USART0.CTRLB &= ~USART_TXEN_bm;
    USART0.CTRLB &= ~USART_RXEN_bm;

    PORT_RXTX.DIRCLR = TX_PIN;  /* reset pinmode */
    PORT_RXTX.DIRCLR = RX_PIN;
}

void USART::_print_char(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm));
    USART0.TXDATAL = c;
}

void USART::print(char *msg) {
    uint8_t i=0;
    while (msg[i] != 0) {
        _print_char(msg[i]);
        i++;
    }
}

void USART::println(char *msg) {
    print(msg);
    PRINT_EOL;
}

void USART::println(void) {
    PRINT_EOL;
}

void USART::print(int8_t value) {
    print(value, DEC);
}

void USART::print(int8_t value, int8_t idx) {
    char c[5];
    itoa(value, c, idx);
    print(c);
}

void USART::println(int8_t value) {
    print(value);
    PRINT_EOL;
}

void USART::println(int8_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}

void USART::print(uint8_t value) {
    print(value, DEC);
}

void USART::print(uint8_t value, int8_t idx) {
    char c[4];
    utoa(value, c, idx);
    print(c);
}

void USART::println(uint8_t value) {
    print(value);
    PRINT_EOL;
}

void USART::println(uint8_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}

void USART::print(int16_t value) {
    print(value, DEC);
}

void USART::print(int16_t value, int8_t idx) {
    char c[7];
    ltoa(value, c, idx);
    print(c);
}

void USART::println(int16_t value) {
    print(value);
    PRINT_EOL;
}

void USART::println(int16_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}

void USART::print(uint16_t value) {
    print(value, DEC);
}

void USART::print(uint16_t value, int8_t idx) {
    char c[6];
    ultoa(value, c, idx);
    print(c);
}

void USART::println(uint16_t value) {
    print(value);
    PRINT_EOL;
}

void USART::println(uint16_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}

void USART::print(int32_t value) {
    print(value, DEC);
}

void USART::print(int32_t value, int8_t idx) {
    char c[12];
    ltoa(value, c, idx);
    print(c);
}

void USART::println(int32_t value) {
    print(value, DEC);
    PRINT_EOL;
}

void USART::println(int32_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}

void USART::print(uint32_t value) {
    print(value, DEC);
}

void USART::print(uint32_t value, int8_t idx) {
    char c[12];
    ultoa(value, c, idx);
    print(c);
}

void USART::println(uint32_t value) {
    println(value, DEC);
}

void USART::println(uint32_t value, int8_t idx) {
    print(value, idx);
    PRINT_EOL;
}