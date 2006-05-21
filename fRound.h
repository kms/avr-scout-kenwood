/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#ifndef _FROUND_H
#define _FROUND_H 1

#include <stdint.h>

typedef struct {
    uint32_t lower;
    uint8_t step;
} freq_bands;

uint32_t roundFreq(uint32_t freq);
#endif
