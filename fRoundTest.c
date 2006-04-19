#include <check.h>
#include "fRound.h"
#include "testSuites.h"

START_TEST(test_simple) {
    
} END_TEST

Suite *fRound_suite(void) {
    Suite *s = suite_create("fRound");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_simple);

    return s;
}
