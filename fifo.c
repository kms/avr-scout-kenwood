#include "fifo.h"

uint8_t isFifoFull(fifo *c) {
    if (((c->write + 1) % c->size) == c->read) {
	return 1;
    } else {
	return 0;
    }
}


uint8_t isFifoEmpty(fifo *c) {
    if (c->write == c->read) {
	return 1;
    } else {
	return 0;
    }
} 

uint8_t fifoGet(fifo *c) {
    c->read = (c->read + 1) % c->size;
    return c->buffer[c->read];
}       

void fifoPut(fifo *c, uint8_t p) {
    c->write = (c->write + 1) % c->size;
    *c->buffer[c->write] = p;
}

fifo* fifoCreate(uint8_t size) {
    fifo *f = (fifo*) malloc(sizeof(fifo));

    f->buffer = (uint8_t*) malloc(size * sizeof(uint8_t));
    f->read = 0;
    f->write = 0;

    return f;
}   
