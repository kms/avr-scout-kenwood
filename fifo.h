/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#ifndef _FIFO_H
#define _FIFO_H 1

#include <stdint.h>

typedef struct {
    volatile uint8_t read;
    volatile uint8_t write;
	volatile uint8_t unconsumed;
    uint8_t capacity;
    uint8_t buffer[0];
} fifo;

uint8_t isFifoFull(fifo *c);
uint8_t isFifoEmpty(fifo *c);
uint8_t fifoSize(fifo *c);
uint8_t fifoGet(fifo *c);
void fifoPut(fifo *c, uint8_t p);
fifo* fifoCreate(uint8_t capacity);
#endif
