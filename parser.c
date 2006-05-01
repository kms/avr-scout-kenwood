#include <stdlib.h>
#include "parser.h"
#include "fifo.h"

parser* createParser() {
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
	    } else {
		resetParser(p);
	    }
	    break;
	case F:
	    if (c == 'F') {
		p->state = DIGITS;
	    } else {
		resetParser(p);
	    }
	    break;
	case DIGITS:
	    if ((c >= 48) && (c <= 57) && (p->digits < 10)) {
		p->digit[p->digits] = c - 48;
		p->digits++;
		if (p->digits == 10) {
		    p->state = CARRIAGE_RETURN;
		}
	    } else {
		resetParser(p);
	    }
	    break;
	case CARRIAGE_RETURN:
	    if (c == '\r') {
		p->state = NEWLINE;
	    } else {
		resetParser(p);
	    }
	    break;
	case NEWLINE:
	    if (c == '\n') {
		p->state = COMPLETE;
	    } else {
		resetParser(p);
	    }
	    break;
	case COMPLETE:
	    break;
	default:
	    resetParser(p);
	    break;
    }
}
