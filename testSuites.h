/* avr-scout-kenwood
 * Scout output to Kenwood tuning commands.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://picofarad.org/
 * Licensed under the GNU GPL 2.0 or later.
 */

#ifndef _TESTSUITES_H
#define _TESTSUITES_H 1

#include <check.h>

Suite *fifo_suite(void);
Suite *fRound_suite(void);
Suite *parser_suite(void);

#endif
