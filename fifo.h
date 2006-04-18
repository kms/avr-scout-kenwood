#include <stdlib.h>

#define uint8_t int

typedef struct {
    uint8_t size;
    volatile uint8_t read;
    volatile uint8_t write;
    uint8_t *buffer;
} fifo;

uint8_t isFifoFull(fifo *c);
uint8_t isFifoEmpty(fifo *c);
uint8_t fifoGet(fifo *c);
void fifoPut(fifo *c, uint8_t p);
fifo* fifoCreate(uint8_t size);
