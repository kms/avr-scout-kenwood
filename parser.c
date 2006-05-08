/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "fifo.h"

parser* createParser(void) {
    parser *p = malloc(sizeof(parser));

    p->state = R;
    p->digits = 0;

    return p;
}

void resetParser(parser *p) {
    p->state = R;
    p->digits = 0;
}

void parseChar(parser *p, uint8_t c) {
    switch (p->state) {
	case R:
	    if (c == 'R') {
		p->state = F;
	    }
	    break;
	case F:
	    if (c == 'F') {
		p->state = DIGITS;
	    } else {
		resetParser(p);
		parseChar(p, c);
	    }
	    break;
	case DIGITS:
	    if (isdigit(c) && (p->digits < 10)) {
		p->digit[p->digits] = c - '0';
		p->digits++;
		if (p->digits == 10) {
		    p->state = CARRIAGE_RETURN;
		}
	    } else {
		resetParser(p);
		parseChar(p, c);
	    }
	    break;
	case CARRIAGE_RETURN:
	    if (c == '\r') {
		p->state = NEWLINE;
	    } else {
		resetParser(p);
		parseChar(p, c);
	    }
	    break;
	case NEWLINE:
	    if (c == '\n') {
		p->state = COMPLETE;
	    } else {
		resetParser(p);
		parseChar(p, c);
	    }
	    break;
	case COMPLETE:
	    break;
	default:
	    resetParser(p);
	    break;
    }
}

uint32_t parseInteger(parser *p) {
    uint32_t i = p->digit[0];
    uint8_t j;

    for (j = 1; j < 10; j++) {
	i = (i * 10) + p->digit[j];
    }

    return i;
}

void padFreq(char *s) {

}
