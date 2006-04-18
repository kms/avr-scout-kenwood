#include "fifo.h"

uint8_t isFifoFull(fifo *c) {
    if (((c->write + 1) % BUFFER_SIZE) == c->read) {
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
    c->read = (c->read + 1) % BUFFER_SIZE;
    return c->buffer[c->read];
}       

void fifoPut(fifo *c, uint8_t p) {
    c->write = (c->write + 1) % BUFFER_SIZE;
    c->buffer[c->write] = p;
}
