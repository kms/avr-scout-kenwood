#include <check.h>
#include "parser.h"
#include "testSuites.h"

START_TEST(test_createParser) {
    parser *p = createParser();
    fail_if(p->state != R);
    fail_if(p->digits != 0);
} END_TEST

START_TEST(test_parseChar) {
    parser *p = createParser();
    fail_if(p == NULL);

    parseChar(p, 'R');
    fail_if(p->state != F);
    parseChar(p, 'F');
    fail_if(p->state != DIGITS);
    parseChar(p, '0');
    fail_if(p->state != DIGITS);
} END_TEST

START_TEST(test_resetParser) {
    parser *p = createParser();
    parseChar(p, 'R');
    parseChar(p, 'F');
    resetParser(p);
    fail_if(p->state != R);
    fail_if(p->digits != 0);
} END_TEST

Suite *parser_suite(void) {
    Suite *s = suite_create("Parser");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_createParser);
    tcase_add_test(tc_core, test_parseChar);
    tcase_add_test(tc_core, test_resetParser);

    return s;
}
