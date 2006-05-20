/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include <check.h>
#include "fifo.h"
#include "testSuites.h"

START_TEST(test_create) {
    fifo *f;

    f = fifoCreate(16);

    fail_if(isFifoEmpty(f) == 0);
} END_TEST

START_TEST(test_fillup) {
    fifo *f;

    f = fifoCreate(2);

    fail_if(isFifoEmpty(f) == 0);

    fifoPut(f, 0);

    fail_if(isFifoEmpty(f) == 1);

    fifoPut(f, 1);

    fail_if(isFifoEmpty(f) == 1);
} END_TEST

START_TEST(test_overflow) {
    fifo *f;

    f = fifoCreate(2);

    fifoPut(f, 0);
    fifoPut(f, 1);
    fifoPut(f, 2);
    fifoPut(f, 3);
    
    fail_if(fifoGet(f) != 0);
    fail_if(fifoGet(f) != 1);
    fail_if(isFifoEmpty(f) == 0);
} END_TEST

START_TEST(test_putget) {
    fifo *f;

    f = fifoCreate(2);

    fifoPut(f, 0);
    fifoGet(f);
    fail_if(isFifoEmpty(f) == 0);

    fifoPut(f, 0);
    fifoPut(f, 1);
    fail_if(isFifoEmpty(f) == 1);

    fail_if(fifoGet(f) != 0);
    fail_if(isFifoEmpty(f) == 1);

    fail_if(fifoGet(f) != 1);
    fail_if(isFifoEmpty(f) == 0);
} END_TEST

Suite *fifo_suite(void) {
    Suite *s = suite_create("FIFO");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_create);
    tcase_add_test(tc_core, test_fillup);
    tcase_add_test(tc_core, test_overflow);
    tcase_add_test(tc_core, test_putget);

    return s;
}
