/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#define F_CPU 3686400UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "fRound.h"
#include "parser.h"

fifo *c;
parser *p;

void uartTx(char *a) {
    while (*a) {
	while (!(UCSRA & _BV(UDRE))) {
	}

	UDR = *a++;
    }

    // Spin until all data is sent
    while (!(UCSRA & _BV(UDRE))) {
    }
}

int main(void) {
    wdt_reset();
    wdt_disable();

    // Enable pull-ups
    PORTA = 0xff;
    PORTB = 0xff;
    PORTD = 0xff;

    DDRD = _BV(DDD1);

    // UART0
    UCSRB |= _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
    UBRRL = 23;

    // Empty RX FIFO
    while (UCSRA & _BV(RXC)) {
	UDR;
    }

    c = fifoCreate(64);
    p = createParser();

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    sei();

    char freq[16];
    uint32_t u;
    uint16_t z = 0;

    for (;;) {
	if (!isFifoEmpty(c)) {
	    parseChar(p, fifoGet(c));
	}

	if (p->state == COMPLETE) {
	    u = parseInteger(p);
	    resetParser(p);
	    uartTx("FQ ");
		intToPaddedString(roundFreq(u), freq);
	    uartTx(freq);
	    uartTx(",8\r\n");
	    uartTx("LMP 1\r\n");
	}
	_delay_ms(1);
	z++;
	if (z == 1024) {
	    uartTx("LMP 0\r\n");
	    z = 0;
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    while (UCSRA & _BV(RXC)) {
	fifoPut(c, UDR);
    }
}
