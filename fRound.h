/* avr-scout-kenwood
 * Scout output to Kenwood tuning commands.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://22pf.org/
 * Licensed under the GNU GPL 2.0 or later.
 */

#ifndef _FROUND_H
#define _FROUND_H 1

#include <stdint.h>

typedef struct {
    uint32_t lower;
    uint8_t step;
} freq_bands;

typedef struct {
    uint32_t step:17;
} freq_steps;

uint32_t roundFreq(uint32_t freq);
#endif
