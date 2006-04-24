
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "fifo.h"
#include "fRound.h"
#include "parser.h"

fifo c;

void uartTx(uint8_t a[], uint8_t len) {
    uint8_t i;

    // TX contents of array
    for (i = 0; i < len; i++) {
	while (!(UCSR0A & _BV(UDRE0))) {
	}

	UDR0 = a[i];
    }

    // Spin until all data is sent
    while (!(UCSR0A & _BV(UDRE0))) {
    }
}

void delay_1s() {
    uint8_t i;

    for (i = 0; i < 100; i++) {
	_delay_ms(10);
    }
}

int main(void) {
    wdt_reset();
    wdt_disable();

    c.read = 0;
    c.write = 0;

    // Enable pull-ups
    PORTB = 0xff;
    PORTC = 0xff;
    PORTD = 0xff;

    DDRD = _BV(DDD1);

    // UART0
    UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    UBRR0L = 95;

    // Empty RX FIFO
    while (UCSR0A & _BV(RXC0)) {
	uint8_t c = UDR0;
    }

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    uint8_t l[] = {'l', 'a'};
    uartTx(l, 2);

    sei();

    fifoPut(&c, 'd');
    fifoPut(&c, 'i');

    for (;;) {
	if (!isFifoEmpty(&c)) {
	    while (!(UCSR0A & _BV(UDRE0))) {
	    }

	    UDR0 = fifoGet(&c);

	    // Spin until all data is sent
	    while (!(UCSR0A & _BV(UDRE0))) {
	    }
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    fifoPut(&c, 'x');
    while (UCSR0A & _BV(RXC0)) {
	uint8_t d = UDR0;
	fifoPut(&c, d);
    }
}

