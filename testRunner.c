/*
 * Scout -> Kenwood
 * Karl-Martin Skontorp LA9PMA <kms@skontorp.net>
 * $Id$
 */

#include <stdlib.h>
#include "testSuites.h"

int main(void) {
  int nf;
  
  SRunner *sr = srunner_create(fifo_suite());
  
  srunner_add_suite(sr, fRound_suite());
  srunner_add_suite(sr, parser_suite());
  
  srunner_run_all(sr, CK_NORMAL);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
