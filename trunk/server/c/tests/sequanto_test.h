#include <check.h>
#include <math.h>

#include <sequanto/stream.h>

/*
  TCase *tc_core = tcase_create ("test_value_init");
  tcase_add_test (tc_core, test_value_init);
  suite_add_tcase (s, tc_core);
*/

#define SQ_TEST_CASE(name) { TCase *tc = tcase_create(#name); tcase_add_test(tc, name); suite_add_tcase(s, tc); }

#define SQ_TEST_SUITE(suite_function, suite_name) Suite * suite_function (void) { Suite * s = suite_create(suite_name);
#define SQ_END_SUITE() return s; }

#define _ck_assert_float_with_epsilon(X, O, Y, epsilon) ck_assert_msg(fabs(fabs((double) X) - fabs((double) Y)) < (epsilon), "Assertion '"#X#O#Y"' failed: "#X"==%f, "#Y"==%f", (double) X, (double) Y)
#define _ck_assert_float(X, O, Y) _ck_assert_float_with_epsilon(X, O, Y, 0.0001)
#define ck_assert_float_eq(X, Y) _ck_assert_float(X, ==, Y) 
#define ck_assert_float_ne(X, Y) _ck_assert_float(X, !=, Y) 

void sq_stream_unit_test_push_read ( SQStream * _stream, SQByte _value );
