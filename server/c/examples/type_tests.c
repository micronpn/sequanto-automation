#include <sequanto/types.h>
#include <sequanto/log.h>

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
static float floatValue;
static double doubleValue;
static SQBool boolValue;

//#include "test_server_automation.h"

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
}

signed long type_test_signed_long_get ( void )
{
   sq_logf ( "Returning value of signed long: %i", signedLongValue );
   return signedLongValue;
}

void type_test_signed_long_set ( signed long _value )
{
   sq_logf ( "Setting value of signed long value to %i (was %i)", _value, signedLongValue );
   signedLongValue = _value;
}

long type_test_long_get ( void )
{
   sq_logf ( "Returning value of long: %i", longValue );
   return longValue;
}

void type_test_long_set ( long _value )
{
   sq_logf ( "Setting value of long value to %i (was %i)", _value, longValue );
   longValue = _value;
}

unsigned long type_test_unsigned_long_get ( void )
{
   sq_logf ( "Returning value of unsigned long: %i", unsignedLongValue );
   return unsignedLongValue;
}

void type_test_unsigned_long_set ( unsigned long _value )
{
   sq_logf ( "Setting value of unsigned long value to %i (was %i)", _value, unsignedLongValue );
   unsignedLongValue = _value;
}

signed int type_test_signed_integer_get ( void )
{
   sq_logf ( "Returning value of signed int: %i", signedIntegerValue );
   return signedIntegerValue;
}

void type_test_signed_integer_set ( signed int _value )
{
   sq_logf ( "Setting value of signed int value to %i (was %i)", _value, signedIntegerValue );
   signedIntegerValue = _value;
   //sq_type_tests_properties_integer_updated ( NULL, _value );
}

int type_test_integer_get ( void )
{
   sq_logf ( "Returning value of int: %i", integerValue );
   return integerValue;
}

void type_test_integer_set ( int _value )
{
   sq_logf ( "Setting value of int value to %i (was %i)", _value, integerValue );
   integerValue = _value;
   //sq_type_tests_properties_integer_updated ( NULL, _value );
}

unsigned int type_test_unsigned_integer_get ( void )
{
   sq_logf ( "Returning value of unsigned int: %i", unsignedIntegerValue );
   return unsignedIntegerValue;
}

void type_test_unsigned_integer_set ( unsigned int _value )
{
   sq_logf ( "Setting value of unsigned int value to %i (was %i)", _value, unsignedIntegerValue );
   unsignedIntegerValue = _value;
}

signed short type_test_signed_short_get ( void )
{
   sq_logf ( "Returning value of signed short: %i", signedShortValue );
   return signedShortValue;
}

void type_test_signed_short_set ( signed short _value )
{
   sq_logf ( "Setting value of signed short value to %i (was %i)", _value, signedShortValue );
   signedShortValue = _value;
}

short type_test_short_get ( void )
{
   sq_logf ( "Returning value of short: %i", shortValue );
   return shortValue;
}

void type_test_short_set ( short _value )
{
sq_logf ( "Setting value of short value to %i (was %i)", _value, shortValue );
   shortValue = _value;
}

unsigned short type_test_unsigned_short_get ( void )
{
   sq_logf ( "Returning value of unsigned short: %i", unsignedShortValue );
   return unsignedShortValue;
}

void type_test_unsigned_short_set ( unsigned short _value )
{
sq_logf ( "Setting value of unsigned short value to %i (was %i)", _value, unsignedShortValue );
   unsignedShortValue = _value;
}

signed char type_test_signed_char_get ( void )
{
   sq_logf ( "Returning value of signed char: %i", signedCharValue );
   return signedCharValue;
}

void type_test_signed_char_set ( signed char _value )
{
sq_logf ( "Setting value of signed char value to %i (was %i)", _value, signedCharValue );
   signedCharValue = _value;
}

char type_test_char_get ( void )
{
   sq_logf ( "Returning value of char: %i", charValue );
   return charValue;
}

void type_test_char_set ( char _value )
{
sq_logf ( "Setting value of char value to %i (was %i)", _value, charValue );
   charValue = _value;
}

unsigned char type_test_unsigned_char_get ( void )
{
   sq_logf ( "Returning value unsigned char: %i", unsignedCharValue );
   return unsignedCharValue;
}

void type_test_unsigned_char_set ( unsigned char _value )
{
sq_logf ( "Setting value of unsigned char value to %i (was %i)", _value, unsignedCharValue );
   unsignedCharValue = _value;
}

float type_test_float_get ( void )
{
   sq_logf ( "Returning value of float: %f", floatValue );
   return floatValue;
}

void type_test_float_set ( float _value )
{
   sq_logf ( "Setting value of float value to %f (was %f)", _value, floatValue );
   floatValue = _value;
}

double type_test_double_get ( void )
{
   sq_logf ( "Returning value of double: %f", doubleValue );
   return doubleValue;
}

void type_test_double_set ( double _value )
{
sq_logf ( "Setting value of double value to %f (was %f)", _value, doubleValue );
   doubleValue = _value;
}

SQBool type_test_bool_get ( void )
{
   sq_logf ( "Returning value of bool: %b", boolValue );
   return boolValue;
}

void type_test_bool_set ( SQBool _value )
{
   sq_logf ( "Setting value of bool value to %b (was %b)", _value, boolValue );
   boolValue = _value;
}
