
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "fRound.h"
#include "parser.h"
#include "fifo.h"

fifo *f;
parser *p;

void uartTxSingle(char a) {
    while (!(UCSR0A & _BV(UDRE0))) {
    }

    UDR0 = a;

    while (!(UCSR0A & _BV(UDRE0))) {
    }
}

void uartTx(char *a) {
    // TX contents of array
    while (*a) {
	while (!(UCSR0A & _BV(UDRE0))) {
	}

	UDR0 = *a++;
    }

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

    f = fifoCreate(64);
    p = createParser();

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    sei();

    char str[64];
    uint32_t u;

    for (;;) {
	if (!isFifoEmpty(f)) {
	    parseChar(p, fifoGet(f));
	}

	if (p->state == COMPLETE) {
	    u = parseInteger(p);
	    sprintf(str, "\r\nFQ %li,3 (%li)\r\n", roundFreq(u), u);
	    uartTx(str);
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    uint8_t c;
    
    while (UCSR0A & _BV(RXC0)) {
	c = UDR0;
	fifoPut(f, c);
	uartTxSingle(c);
    }
}
