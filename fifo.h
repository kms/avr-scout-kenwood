#include <stdlib.h>

#define BUFFER_SIZE 32

#define uint8_t int

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t read;
    volatile uint8_t write;
} fifo;

uint8_t isFifoFull(fifo *c);
uint8_t isFifoEmpty(fifo *c);
uint8_t fifoGet(fifo *c);
void fifoPut(fifo *c, uint8_t p);
