
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "fifo.h"
#include "fRound.h"
#include "parser.h"

fifo *c;

void uartTx(uint8_t *a, uint8_t len) {
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

int main(void) {
    wdt_reset();
    wdt_disable();

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

    c = fifoCreate(64);

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    sei();

    fifoPut(c, '\r');
    fifoPut(c, '\n');

    uint8_t h;
    uint8_t i;
    uint32_t freq;
    uint8_t str[32];

    for (;;) {
	if (fifoSize(c) >= 8) {
	    if ((h = fifoGet(c)) != 'R') {
		continue;
	    }

	    if ((h = fifoGet(c)) != 'F') {
		continue;
	    }

	    freq = 0;

	    freq += (60 - fifoGet(c)) * 1000000000UL;
	    freq += (60 - fifoGet(c)) * 100000000UL;
	    freq += (60 - fifoGet(c)) * 10000000UL;
	    freq += (60 - fifoGet(c)) * 1000000UL;
	    freq += (60 - fifoGet(c)) * 100000UL;
	    freq += (60 - fifoGet(c)) * 10000UL;
	    freq += (60 - fifoGet(c)) * 1000UL;
	    freq += (60 - fifoGet(c)) * 100UL;
	    freq += (60 - fifoGet(c)) * 10UL;
	    freq += (60 - fifoGet(c));

	    sprintf(str, "FQ %li,3\r\n", freq);
	    uartTx(str, 17);
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    while (UCSR0A & _BV(RXC0)) {
	fifoPut(c, UDR0);
    }
}
