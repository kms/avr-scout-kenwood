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
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "fRound.h"
#include "parser.h"

fifo *c;
parser *p;

void uartTx(const char *a) {
    while (*a) {
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = *a++;
    }
}

void uartTx_P(const char *a) {
    while (pgm_read_byte_near(a)) {
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = pgm_read_byte_near(a++);
    }
}

int main(void) {
    wdt_reset();
    wdt_disable();

    //    CLKPR = _BV(CLKPCE);
    //    CLKPR = _BV(CLKPS0);

    // Enable pull-ups
    PORTA = 0xff;
    PORTB = 0xff;
    PORTD = 0xff;

    DDRD = _BV(DDD1);

    DIDR = _BV(AIN1D) | _BV(AIN0D);
    ACSR = _BV(ACD);
    
    // USART
    UCSRB = _BV(TXEN) | _BV(RXCIE) | _BV(RXEN);
    UBRRL = 25;

    c = fifoCreate(20);
    p = createParser();

    char freq[12];
    uint16_t z = 0;

    // Go!
    sei();

    for (;;) {
	if (!isFifoEmpty(c)) {
	    parseChar(p, fifoGet(c));
	}

	if (p->state == COMPLETE) {
	    uartTx_P(PSTR("FQ "));
	    intToPaddedString(roundFreq(parseInteger(p)), freq);
	    uartTx(freq);
	    uartTx_P(PSTR(",5\r\nLMP 1\r\n"));
	    z = 0;
	    resetParser(p);
	}

	if (z++ == 4096) {
	    uartTx_P(PSTR("LMP 0\r\n"));
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
