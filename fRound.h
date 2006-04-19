#include <stdint.h>

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

uint8_t findBand(uint32_t freq);
uint32_t roundFreq(uint32_t freq);
