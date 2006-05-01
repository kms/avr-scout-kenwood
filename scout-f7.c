
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
parser *p;

void uartTx(char *a, uint8_t len) {
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
    p = createParser();

    // Go!
    wdt_reset();
    //wdt_enable(WDTO_500MS);

    sei();

    char str[32];

    for (;;) {
	if (p->state == COMPLETE) {
	    sprintf(str, "FQ %li,3\r\n", roundFreq(parseInteger(p)));
	    uartTx(str, 17);
	    resetParser(p);
	}
    }

    return 0;
}

ISR(USART_RX_vect) {
    while (UCSR0A & _BV(RXC0)) {
	parseChar(p, UDR0);
    }
}
