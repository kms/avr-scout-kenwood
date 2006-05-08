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
    uint32_t i = 0;
    
    i += p->digit[0] * 1000000000UL;
    i += p->digit[1] * 100000000UL;
    i += p->digit[2] * 10000000UL;
    i += p->digit[3] * 1000000UL;
    i += p->digit[4] * 100000UL;
    i += p->digit[5] * 10000UL;
    i += p->digit[6] * 1000;
    i += p->digit[7] * 100;
    i += p->digit[8] * 10;
    i += p->digit[9];

    return i;
}

void padFreq(char *s) {
    
}
