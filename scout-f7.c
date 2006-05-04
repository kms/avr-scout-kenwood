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
#include "fifo.h"
#include "fRound.h"
#include "parser.h"

fifo *c;
parser *p;

void uartTx(char *a) {
    while (*a) {
	while (!(UCSR0A & _BV(UDRE0))) {
	}

	UDR0 = *a++;
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
    UBRR0L = 23;

    // Empty RX FIFO
    while (UCSR0A & _BV(RXC0)) {
	UDR0;
    }

    c = fifoCreate(64);
    p = createParser();

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    sei();

    uartTx("# Scout -> Kenwood $Rev$ by LA9PMA <kms@skontorp.net>\r\n"
	    "# Compiled: " __TIMESTAMP_STRING__ "\r\n"
	    "# gcc " __VERSION__
	    " avr-libc " __AVR_LIBC_VERSION_STRING__ "\r\n");

    char freq[16];
    uint32_t u;

    for (;;) {
	if (!isFifoEmpty(c)) {
	    parseChar(p, fifoGet(c));
	}

	if (p->state == COMPLETE) {
	    u = parseInteger(p);
	    resetParser(p);
	    uartTx("FQ ");
	    ultoa(roundFreq(u), freq, 10);
	    uartTx(freq);
	    uartTx(",3 (");
	    ultoa(u, freq, 10);
	    uartTx(freq);
	    uartTx(")\r\n");
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    while (UCSR0A & _BV(RXC0)) {
	fifoPut(c, UDR0);
    }
}


