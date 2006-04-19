#include <stdlib.h>
#include "fifo.h"

uint8_t isFifoFull(fifo *c) {
    return ((c->write + 1) % c->size) == c->read;
}

uint8_t isFifoEmpty(fifo *c) {
    return c->write == c->read;
} 

uint8_t fifoGet(fifo *c) {
    c->read = (c->read + 1) % c->size;
    return c->buffer[c->read];
}       

void fifoPut(fifo *c, uint8_t p) {
    c->write = (c->write + 1) % c->size;
    c->buffer[c->write] = p;
}

fifo* fifoCreate(uint8_t size) {
    fifo *f = malloc(sizeof(fifo));

    if (f == NULL) {
	for (;;) {
	}
    }

    f->buffer = malloc(size * sizeof(uint8_t));

    if (f->buffer == NULL) {
	for (;;) {
	}
    }
    
    f->read = 0;
    f->write = 0;
    f->size = size + 1;

    return f;
}   
