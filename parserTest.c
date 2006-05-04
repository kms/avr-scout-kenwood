#include <check.h>
#include "parser.h"
#include "testSuites.h"

START_TEST(test_createParser) {
    parser *p = createParser();
    fail_if(p->state != R);
    fail_if(p->digits != 0);
} END_TEST

START_TEST(test_parseChar) {
    int i;

    parser *p = createParser();

    parseChar(p, 'R');
    fail_if(p->state != F);
    fail_if(p->digits != 0);
    
    parseChar(p, 'F');
    fail_if(p->state != DIGITS);
    fail_if(p->digits != 0);

    for (i = 1; i <= 9; i++) {
	parseChar(p, 48 + i);
	fail_if(p->state != DIGITS);
	fail_if(p->digits != i);
    }

    parseChar(p, '0');
    fail_if(p->state != CARRIAGE_RETURN);
    fail_if(p->digits != 10);

    parseChar(p, '\r');
    fail_if(p->state != NEWLINE);
    fail_if(p->digits != 10);

    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(p->digits != 10);

    fail_if(p->digit[0] != 1);
    fail_if(p->digit[1] != 2);
    fail_if(p->digit[2] != 3);
    fail_if(p->digit[3] != 4);
    fail_if(p->digit[4] != 5);
    fail_if(p->digit[5] != 6);
    fail_if(p->digit[6] != 7);
    fail_if(p->digit[7] != 8);
    fail_if(p->digit[8] != 9);
    fail_if(p->digit[9] != 0);
} END_TEST

START_TEST(test_parseCharTooFewDigits1) {
    parser *p = createParser();
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, 'R');
    fail_if(p->state != F);
} END_TEST

START_TEST(test_parseCharTooFewDigits2) {
    parser *p = createParser();
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '1');
    parseChar(p, '2');
    parseChar(p, 'R');
    fail_if(p->state != F);
} END_TEST

START_TEST(test_parseCharTooManyDigits) {
    parser *p = createParser();
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '1');
    parseChar(p, '2');
    parseChar(p, '3');
    parseChar(p, '4');
    parseChar(p, '5');
    parseChar(p, '6');
    parseChar(p, '7');
    parseChar(p, '8');
    parseChar(p, '9');
    parseChar(p, '0');
    parseChar(p, '1');
    fail_if(p->state != R);
} END_TEST

START_TEST(test_resetParser) {
    parser *p = createParser();
    parseChar(p, 'R');
    parseChar(p, 'F');
    resetParser(p);
    fail_if(p->state != R);
    fail_if(p->digits != 0);
} END_TEST

START_TEST(test_parseInteger) {
    parser *p = createParser();
    
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '\r');
    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(parseInteger(p) != 0);
    resetParser(p);
    
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '1');
    parseChar(p, '\r');
    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(parseInteger(p) != 1);
    resetParser(p);
    
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '0');
    parseChar(p, '1');
    parseChar(p, '2');
    parseChar(p, '3');
    parseChar(p, '4');
    parseChar(p, '5');
    parseChar(p, '6');
    parseChar(p, '7');
    parseChar(p, '8');
    parseChar(p, '9');
    parseChar(p, '\r');
    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(parseInteger(p) != 123456789);
    resetParser(p);
    
    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '3');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '9');
    parseChar(p, '\r');
    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(parseInteger(p) != 3999999999UL);
    resetParser(p);

    parseChar(p, 'R');
    parseChar(p, 'F');
    parseChar(p, '0');
    parseChar(p, '6');
    parseChar(p, '2');
    parseChar(p, '1');
    parseChar(p, '5');
    parseChar(p, '8');
    parseChar(p, '5');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '0');
    parseChar(p, '\r');
    parseChar(p, '\n');
    fail_if(p->state != COMPLETE);
    fail_if(parseInteger(p) != 621585000);
} END_TEST

Suite *parser_suite(void) {
    Suite *s = suite_create("Parser");
    TCase *tc_core = tcase_create("Core");

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_createParser);
    tcase_add_test(tc_core, test_parseChar);
    tcase_add_test(tc_core, test_resetParser);
    tcase_add_test(tc_core, test_parseCharTooFewDigits1);
    tcase_add_test(tc_core, test_parseCharTooFewDigits2);
    tcase_add_test(tc_core, test_parseCharTooManyDigits);
    tcase_add_test(tc_core, test_parseInteger);

    return s;
}
