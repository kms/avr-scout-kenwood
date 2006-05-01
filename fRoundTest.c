#include <check.h>
#include "fRound.h"
#include "testSuites.h"

START_TEST(test_simple) {
    fail_if(roundFreq(145600001UL) != 145600000UL);
    fail_if(roundFreq(145600020UL) != 145600000UL);
    fail_if(roundFreq(145600300UL) != 145600000UL);
    fail_if(roundFreq(145604000UL) != 145600000UL);
    fail_if(roundFreq(145605000UL) != 145600000UL);
    fail_if(roundFreq(145606000UL) != 145600000UL);
    fail_if(roundFreq(145607000UL) != 145612500UL);
    fail_if(roundFreq(145618749UL) != 145612500UL);
    fail_if(roundFreq(145618750UL) != 145625000UL);
    fail_if(roundFreq(145618751UL) != 145625000UL);
} END_TEST

Suite *fRound_suite(void) {
    Suite *s = suite_create("fRound");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_simple);

    return s;
}
