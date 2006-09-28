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

void uartTx_P(PGM_P a) {
    while (pgm_read_byte_near(a)) {
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = pgm_read_byte_near(a++);
    }
}

void controlLoop(void) {
    char freq[12];

    for (;;) {
	if (!isFifoEmpty(c)) {
	    parseChar(p, fifoGet(c));
	}

	if (p->state == COMPLETE) {
	    uartTx_P(PSTR("FQ "));
	    intToPaddedString(roundFreq(parseInteger(p)), freq);
	    uartTx(freq);
	    uartTx_P(PSTR(",5\r\n"));
	    resetParser(p);
	}

	wdt_reset();
    }
}

ISR(USART_RX_vect) {
    while (UCSRA & _BV(RXC)) {
	fifoPut(c, UDR);
    }
}

int main(void) {
    wdt_reset();
    wdt_disable();
    //wdt_enable(WDTO_2S);

    //CLKPR = _BV(CLKPCE);
    //CLKPR = _BV(CLKPS1);

    PORTA = 0xff;
    PORTB = 0xff;
    PORTD = 0xff;

    DDRD = _BV(DDD1);

    DIDR = _BV(AIN1D) | _BV(AIN0D);
    ACSR = _BV(ACD);
    
    UCSRB = _BV(TXEN) | _BV(RXCIE) | _BV(RXEN);
    UBRRL = 23;
    
    c = fifoCreate(16);
    p = createParser();    
    
    sei();

    controlLoop();

    return 0;
}
