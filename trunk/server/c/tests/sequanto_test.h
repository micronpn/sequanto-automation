#include <check.h>


/*
  TCase *tc_core = tcase_create ("test_value_init");
  tcase_add_test (tc_core, test_value_init);
  suite_add_tcase (s, tc_core);
*/

#define SQ_TEST_CASE(name) { TCase *tc = tcase_create(#name); tcase_add_test(tc, name); suite_add_tcase(s, tc); }

#define SQ_TEST_SUITE(suite_function, suite_name) Suite * suite_function (void) { Suite * s = suite_create(suite_name);
#define SQ_END_SUITE() return s; }
