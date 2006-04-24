#include <stdlib.h>
#include "fifo.h"

uint8_t isFifoFull(fifo *c) {
    return ((c->write + 1) % c->capacity) == c->read;
}

uint8_t isFifoEmpty(fifo *c) {
    return c->write == c->read;
} 

uint8_t fifoSize(fifo *c) {
    return c->size;
} 

uint8_t fifoGet(fifo *c) {
    c->size--;
    c->read = (c->read + 1) % c->capacity;
    return c->buffer[c->read];
}       

void fifoPut(fifo *c, uint8_t p) {
    c->size++;
    c->write = (c->write + 1) % c->capacity;
    c->buffer[c->write] = p;
}

fifo* fifoCreate(uint8_t capacity) {
    fifo *f = malloc(sizeof(fifo));

    if (f == NULL) {
	for (;;) {
	}
    }

    f->buffer = malloc(capacity * sizeof(uint8_t));

    if (f->buffer == NULL) {
	for (;;) {
	}
    }
    
    f->read = 0;
    f->write = 0;
    f->size = 0;
    f->capacity = capacity + 1;

    return f;
}   
