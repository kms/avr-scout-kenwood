#include <check.h>
#include "parser.h"
#include "testSuites.h"

START_TEST(test_create) {
    
} END_TEST

Suite *parser_suite(void) {
    Suite *s = suite_create("Parser");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_create);

    return s;
}
