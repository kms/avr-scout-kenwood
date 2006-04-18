#include <check.h>
#include "fifo.h"

START_TEST (test_create)
{
 fifo f;
 
 fail_unless(isFifoEmpty(f) == 1);
 fail_if(isFifoFull(f) == 1);
}
END_TEST
