/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include <stdlib.h>
#include "fRound.h"

#define NUMBER_OF_BANDS 1

freq_bands bands[NUMBER_OF_BANDS] = {
    {0, 12500},
};

uint32_t roundFreq(uint32_t freq) {
    uint8_t band;
    uint16_t r;
    for (band = 1; band < NUMBER_OF_BANDS; band++) {
	if ((bands[band - 1].lower <= freq)
		&& (bands[band].lower > freq)) {
	    break;
	}
    }
	band--;
	r = freq % bands[band].step;

    if (r >= (bands[band].step / 2)) {
	freq += bands[band].step;
    }
	return freq - r;
}
