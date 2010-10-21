#include <sequanto/types.h>
#include <sequanto/log.h>
#include "config.h"

typedef signed short bool_t;

static signed long signedLongValue;
static long longValue;
static unsigned long unsignedLongValue;
static signed int signedIntegerValue;
static int integerValue;
static unsigned int unsignedIntegerValue;
static signed short signedShortValue;
static short shortValue;
static unsigned short unsignedShortValue;
static signed char signedCharValue;
static char charValue;
static unsigned char unsignedCharValue;
#ifdef HAVE_INT64_T
static int64_t int64_tValue;
#endif
static float floatValue;
static double doubleValue;
static SQBool boolValue;
static bool_t bool_tValue;

#include "test_server_automation.h"

void type_test_reset ( void )
{
   signedLongValue = 0;
   longValue = 0;
   unsignedLongValue = 0;
   signedIntegerValue = 0;
   integerValue = 0;
   unsignedIntegerValue = 0;
   signedShortValue = 0;
   shortValue = 0;
   unsignedShortValue =0;
   signedCharValue = 0;
   charValue = 0;
   unsignedCharValue = 0;
   floatValue = 0;
   doubleValue = 0;
   boolValue = SQ_FALSE;
   bool_tValue = 0;
   
   sq_type_tests_properties_signed_long_updated ( signedLongValue );
   sq_type_tests_properties_long_updated ( longValue );
   sq_type_tests_properties_unsigned_long_updated ( unsignedLongValue );
   sq_type_tests_properties_signed_integer_updated ( signedIntegerValue );
   sq_type_tests_properties_integer_updated ( integerValue );
   sq_type_tests_properties_unsigned_integer_updated ( unsignedIntegerValue );
   sq_type_tests_properties_signed_short_updated ( signedShortValue );
   sq_type_tests_properties_short_updated ( shortValue );
   sq_type_tests_properties_unsigned_short_updated ( unsignedShortValue );
   sq_type_tests_properties_signed_char_updated ( signedCharValue );
   sq_type_tests_properties_char_updated ( charValue );
   sq_type_tests_properties_unsigned_char_updated ( unsignedCharValue );
   sq_type_tests_properties_float_updated ( floatValue );
   sq_type_tests_properties_double_updated ( doubleValue );
   sq_type_tests_properties_bool_updated ( boolValue );
   sq_type_tests_properties_bool_t_updated ( bool_tValue );
}

signed long type_test_signed_long_get ( void )
{
#ifndef SQ_ARDUINO
   sq_log_updated ( signedLongValue, "The value of signed long was queries" );
   sq_logf ( "Returning value of signed long: %i", signedLongValue );
#endif
   return signedLongValue;
}

void type_test_signed_long_set ( signed long _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of signed long value to %i (was %i)", _value, signedLongValue );
#endif
   signedLongValue = _value;
   sq_type_tests_properties_signed_long_updated ( signedLongValue );
}

long type_test_long_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of long: %i", longValue );
#endif
   return longValue;
}

void type_test_long_set ( long _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of long value to %i (was %i)", _value, longValue );
#endif
   longValue = _value;
   sq_type_tests_properties_long_updated ( longValue );
}

unsigned long type_test_unsigned_long_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of unsigned long: %i", unsignedLongValue );
#endif
   return unsignedLongValue;
}

void type_test_unsigned_long_set ( unsigned long _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of unsigned long value to %i (was %i)", _value, unsignedLongValue );
#endif
   unsignedLongValue = _value;
   sq_type_tests_properties_unsigned_long_updated ( unsignedLongValue );
}

signed int type_test_signed_integer_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of signed int: %i", signedIntegerValue );
#endif
   return signedIntegerValue;
}

void type_test_signed_integer_set ( signed int _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of signed int value to %i (was %i)", _value, signedIntegerValue );
#endif
   signedIntegerValue = _value;
   //sq_type_tests_properties_integer_updated ( NULL, _value );
   sq_type_tests_properties_signed_integer_updated ( signedIntegerValue );
}

int type_test_integer_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of int: %i", integerValue );
#endif
   return integerValue;
}

void type_test_integer_set ( int _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of int value to %i (was %i)", _value, integerValue );
#endif
   integerValue = _value;
   sq_type_tests_properties_integer_updated ( integerValue );
}

unsigned int type_test_unsigned_integer_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of unsigned int: %i", unsignedIntegerValue );
#endif
   return unsignedIntegerValue;
}

void type_test_unsigned_integer_set ( unsigned int _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of unsigned int value to %i (was %i)", _value, unsignedIntegerValue );
#endif
   unsignedIntegerValue = _value;
   sq_type_tests_properties_unsigned_integer_updated ( unsignedIntegerValue );
}

signed short type_test_signed_short_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of signed short: %i", signedShortValue );
#endif
   return signedShortValue;
}

void type_test_signed_short_set ( signed short _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of signed short value to %i (was %i)", _value, signedShortValue );
#endif
   signedShortValue = _value;
   sq_type_tests_properties_signed_short_updated ( signedShortValue );
}

short type_test_short_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of short: %i", shortValue );
#endif
   return shortValue;
}

void type_test_short_set ( short _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of short value to %i (was %i)", _value, shortValue );
#endif
   shortValue = _value;
   sq_type_tests_properties_short_updated ( shortValue );
}

unsigned short type_test_unsigned_short_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of unsigned short: %i", unsignedShortValue );
#endif
   return unsignedShortValue;
}

void type_test_unsigned_short_set ( unsigned short _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of unsigned short value to %i (was %i)", _value, unsignedShortValue );
#endif
   unsignedShortValue = _value;
   sq_type_tests_properties_unsigned_short_updated ( unsignedShortValue );
}

signed char type_test_signed_char_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of signed char: %i", signedCharValue );
#endif
   return signedCharValue;
}

void type_test_signed_char_set ( signed char _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of signed char value to %i (was %i)", _value, signedCharValue );
#endif
   signedCharValue = _value;
   sq_type_tests_properties_signed_char_updated ( signedCharValue );
}

char type_test_char_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of char: %i", charValue );
#endif
   return charValue;
}

void type_test_char_set ( char _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of char value to %i (was %i)", _value, charValue );
#endif
   charValue = _value;
   sq_type_tests_properties_char_updated ( charValue );
}

unsigned char type_test_unsigned_char_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value unsigned char: %i", unsignedCharValue );
#endif
   return unsignedCharValue;
}

void type_test_unsigned_char_set ( unsigned char _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of unsigned char value to %i (was %i)", _value, unsignedCharValue );
#endif
   unsignedCharValue = _value;
   sq_type_tests_properties_unsigned_char_updated ( unsignedCharValue );
}

#ifdef HAVE_INT64_T
int64_t type_test_int64_t_get ( void )
{
   sq_log_updated ( int64_tValue, "The value of int64_t was queries" );
   sq_logf ( "Returning value of signed long: %i", int64_tValue );
   return int64_tValue;
}

void type_test_int64_t_set ( int64_t _value )
{
   sq_logf ( "Setting value of int64_t value to %i (was %i)", _value, int64_tValue );
   int64_tValue = _value;
   //sq_type_tests_properties_int64_t_updated ( int64_tValue );
}
#endif

float type_test_float_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of float: %f", floatValue );
#endif
   return floatValue;
}

void type_test_float_set ( float _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of float value to %f (was %f)", _value, floatValue );
#endif
   floatValue = _value;
   sq_type_tests_properties_float_updated ( floatValue );
}

double type_test_double_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of double: %f", doubleValue );
#endif
   return doubleValue;
}

void type_test_double_set ( double _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of double value to %f (was %f)", _value, doubleValue );
#endif
   doubleValue = _value;
   sq_type_tests_properties_double_updated ( doubleValue );
}

SQBool type_test_bool_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of bool: %b", boolValue );
#endif
   return boolValue;
}

void type_test_bool_set ( SQBool _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of bool value to %b (was %b)", _value, boolValue );
#endif
   boolValue = _value;
   sq_type_tests_properties_bool_updated ( boolValue );
}

bool_t type_test_bool_t_get ( void )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Returning value of bool_t: %b", bool_tValue );
#endif
   return bool_tValue;
}

void type_test_bool_t_set ( bool_t _value )
{
#ifndef SQ_ARDUINO
   sq_logf ( "Setting value of bool_t value to %b (was %b)", _value, bool_tValue );
#endif
   bool_tValue = _value;
   sq_type_tests_properties_bool_updated ( bool_tValue );
}
