/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include "fRound.h"

#define NUMBER_OF_BANDS 1

freq_bands bands[NUMBER_OF_BANDS] = {
    {0, 0},
};

uint32_t steps[] = {12500};

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

    uint32_t step = steps[bands[band].step];
    r = freq % step;

    if (r >= (step / 2)) {
	freq += step;
    }

    return freq - r;
}
