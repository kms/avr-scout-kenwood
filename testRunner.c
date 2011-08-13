/* avr-scout-kenwood
 * Scout output to Kenwood tuning commands.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://22pf.org/
 * Licensed under the GNU GPL 2.0 or later.
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
