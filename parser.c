/* avr-scout-kenwood
 * Scout output to Kenwood tuning commands.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://22pf.org/
 * Licensed under the GNU GPL 2.0 or later.
 */

#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define PADSIZE 11

parser* createParser(void) {
    parser *p = malloc(sizeof(parser));
    resetParser(p);
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
            if (c >= '0' && c <= '9' && (p->digits < 10)) {
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

uint32_t parseInteger(const parser *p) {
    uint32_t i = p->digit[0];
    uint8_t j;

    for (j = 1; j < 10; j++) {
        i = (i<<3) + (i<<1) + p->digit[j];
    }

    return i;
}

void intToPaddedString(uint32_t u, char *s) {
    char *tmp = s;
    
    while (u > 0) {
        *tmp++ = (u % 10) + '0';
        u /= 10;
    }
    
    while (tmp - s < PADSIZE) {
        *tmp++ = '0';
    }
    
    *tmp-- = '\0';
    
    while (tmp >= s) {
        uint8_t c = *s;
        *s++ = *tmp;
        *tmp-- = c;
    }
}
