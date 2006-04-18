
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "fifo.h"

fifo c;

typedef struct {
    volatile uint32_t lower;
    volatile uint32_t step;
} freq_bands;

#define NUMBER_OF_BANDS 10

freq_bands bands[NUMBER_OF_BANDS] = {
    {0, 1},
    {10000000, 1},
    {29400000, 10000},
    {30000000, 12500},
    {89000000, 100000},
    {108000000, 25000},
    {130000000, 12500},
    {144000000, 1},
    {144500000, 12500},
    {145000000, 25000},
};

uint8_t findBand(uint32_t freq) {
    uint8_t i;

    for (i = 1; i < NUMBER_OF_BANDS; i++) {
	if ((bands[i - 1].lower <= freq)
		&& (bands[i].lower > freq)) {
	    break;
	}
    }

    return i - 1;
}

uint32_t roundFreq(uint32_t freq) {
    uint8_t band = findBand(freq);
    uint16_t r = freq % bands[band].step;

    if (r == 0) {
	return freq;
    }

    if (r < (bands[band].step / 2)) {
	return freq - r;
    } else {
	return freq + (bands[band].step - r);
    }
}

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
