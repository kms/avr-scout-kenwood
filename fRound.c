#include <stdlib.h>
#include "fRound.h"

uint8_t findBand(uint32_t freq) {
    uint8_t i;

    for (i = 1; i < NUMBER_OF_BANDS; i++) {
	if ((bands[i - 1].lower <= freq)
		&& (bands[i].lower > freq)) {
	    break;
	}
    }

    return i - 1;
}

uint32_t roundFreq(uint32_t freq) {
    uint8_t band = findBand(freq);
    uint16_t r = freq % bands[band].step;

    if (r == 0) {
	return freq;
    }

    if (r < (bands[band].step / 2)) {
	return freq - r;
    } else {
	return freq + (bands[band].step - r);
    }
}
