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
	loop_until_bit_is_clear(UCSRA, UDRE);
	UDR = *a++;
    }
}

int main(void) {
    wdt_reset();
    wdt_disable();

    CLKPR = _BV(CLKPCE);
    CLKPR = _BV(CLKPS0);

    // Enable pull-ups
    PORTA = 0xff;
    PORTB = 0xff;
    PORTD = 0xff;

    DDRD = _BV(DDD1);

    // UART0
    UCSRB |= _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
    UBRRL = 25;

    // Empty RX FIFO
    while (bit_is_set(UCSRA, RXC)) {
	UDR;
    }
    
    uartTx("# Scout->Kenwood $Rev$ <kms@skontorp.net>\r\n"
	    "# Compiled: " __TIMESTAMP_STRING__ "\r\n");

    c = fifoCreate(20);
    p = createParser();

    char freq[16];
    uint32_t u;
    uint16_t z = 0;

    // Go!
    sei();

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
	    uartTx(",5\r\n");
	    uartTx("LMP 1\r\n");
	    z = 0;
	}
	
	if (z++ == 4192) {
	    uartTx("LMP 0\r\n");
	    z = 0;
	}
	
	_delay_ms(1);
    }

    return 0;
}

ISR(USART_RX_vect) {
    while (UCSRA & _BV(RXC)) {
	fifoPut(c, UDR);
    }
}
