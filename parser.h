/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#ifndef _PARSER_H
#define _PARSER_H 1

#include <stdint.h>
#include "fifo.h"

enum states {DIGITS, COMPLETE};

typedef struct {
    enum states state;
    uint8_t digits;
    uint8_t digit[10];
} parser;

parser* createParser(void);
void resetParser(parser *p);
void parseChar(parser *p, uint8_t c);
uint32_t parseInteger(const parser *p);
void intToPaddedString(uint32_t u, char *s);

#endif
