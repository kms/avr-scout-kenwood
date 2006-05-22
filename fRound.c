/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include "fRound.h"

#define NUMBER_OF_BANDS 6

freq_bands bands[NUMBER_OF_BANDS] = {
    {0, 5},
    {1, 5},
    {2, 5},
    {3, 5},
    {4, 5},
    {5, 5},
};

freq_steps steps[] = {
    {1},
    {5000},
    {8333},
    {9000},
    {10000},
    {12500},
    {25000},
    {50000},
    {100000}};

uint32_t roundFreq(uint32_t freq) {
    uint8_t band;
    uint32_t r;

    for (band = 1; band < NUMBER_OF_BANDS; band++) {
	if ((bands[band - 1].lower <= freq)
		&& (bands[band].lower > freq)) {
	    break;
	}
    }

    band--;

    uint32_t step = steps[bands[band].step].step;
    r = freq % step;

    if (r >= (step / 2)) {
	freq += step;
    }

    return freq - r;
}
