/* avr-scout-kenwood
 * Scout output to Kenwood tuning commands.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://22pf.org/
 * Licensed under the GNU GPL 2.0 or later.
 */

#include <stdlib.h>
#include "fifo.h"

uint8_t isFifoEmpty(const fifo *c) {
    return c->unconsumed == 0;
} 

uint8_t fifoGet(fifo *c) {
    c->unconsumed %= c->capacity;
    return c->buffer[(c->write - c->unconsumed--) % c->capacity];
}       

void fifoPut(fifo *c, const uint8_t p) {
    c->buffer[c->write++ % c->capacity] = p;
    c->unconsumed++;
}

fifo* fifoCreate(const uint8_t capacity) {
    fifo *f = malloc(sizeof(fifo) + (capacity * sizeof(uint8_t)));

    f->write = 0;
    f->unconsumed = 0;
    f->capacity = capacity;

    return f;
}   
