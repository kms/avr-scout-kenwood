/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include <stdlib.h>
#include "fifo.h"

uint8_t isFifoEmpty(const fifo *c) {
    return c->unconsumed == 0;
} 

uint8_t fifoGet(fifo *c) {
    c->read = c->read % c->capacity;
    c->unconsumed--;
    return c->buffer[c->read++];
}       

void fifoPut(fifo *c, const uint8_t p) {
    c->write = c->write % c->capacity;
    c->buffer[c->write++] = p;
    c->unconsumed++;
}

fifo* fifoCreate(const uint8_t capacity) {
    fifo *f = malloc(sizeof(fifo) + (capacity * sizeof(uint8_t)));

    f->read = 0;
    f->write = 0;
    f->unconsumed = 0;
    f->capacity = capacity;

    return f;
}   
