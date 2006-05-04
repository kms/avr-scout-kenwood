/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#ifndef _FROUND_H
#define _FROUND_H 1

#include <stdint.h>

typedef struct {
    volatile uint32_t lower;
    volatile uint32_t step;
} freq_bands;

uint8_t findBand(uint32_t freq);
uint32_t roundFreq(uint32_t freq);
#endif
