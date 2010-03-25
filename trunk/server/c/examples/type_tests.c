#include <sequanto/types.h>

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

void type_test_reset ()
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

   boolValue = SQ_TRUE;
}

signed long type_test_signed_long_get ()
{
   return signedLongValue;
}

void type_test_signed_long_set ( signed long _value )
{
   signedLongValue = _value;
}

long type_test_long_get ()
{
   return longValue;
}

void type_test_long_set ( long _value )
{
   longValue = _value;
}

unsigned long type_test_unsigned_long_get ()
{
   return unsignedLongValue;
}

void type_test_unsigned_long_set ( unsigned long _value )
{
   unsignedLongValue = _value;
}

signed int type_test_signed_integer_get ()
{
   return signedIntegerValue;
}

void type_test_signed_integer_set ( signed int _value )
{
   signedIntegerValue = _value;
   //sq_type_tests_properties_integer_updated ( NULL, _value );
}

int type_test_integer_get ()
{
   return integerValue;
}

void type_test_integer_set ( int _value )
{
   integerValue = _value;
   //sq_type_tests_properties_integer_updated ( NULL, _value );
}

unsigned int type_test_unsigned_integer_get ()
{
   return unsignedIntegerValue;
}

void type_test_unsigned_integer_set ( unsigned int _value )
{
   unsignedIntegerValue = _value;
}

signed short type_test_signed_short_get ()
{
   return signedShortValue;
}

void type_test_signed_short_set ( signed short _value )
{
   signedShortValue = _value;
}

short type_test_short_get ()
{
   return shortValue;
}

void type_test_short_set ( short _value )
{
   shortValue = _value;
}

unsigned short type_test_unsigned_short_get ()
{
   return unsignedShortValue;
}

void type_test_unsigned_short_set ( unsigned short _value )
{
   unsignedShortValue = _value;
}

signed char type_test_signed_char_get ()
{
   return signedCharValue;
}

void type_test_signed_char_set ( signed char _value )
{
   signedCharValue = _value;
}

char type_test_char_get ()
{
   return charValue;
}

void type_test_char_set ( char _value )
{
   charValue = _value;
}

unsigned char type_test_unsigned_char_get ()
{
   return unsignedCharValue;
}

void type_test_unsigned_char_set ( unsigned char _value )
{
   unsignedCharValue = _value;
}

float type_test_float_get ()
{
   return floatValue;
}

void type_test_float_set ( float _value )
{
   floatValue = _value;
}

double type_test_double_get ()
{
   return doubleValue;
}

void type_test_double_set ( double _value )
{
   doubleValue = _value;
}

SQBool type_test_bool_get ()
{
   return boolValue;
}

void type_test_bool_set ( SQBool _value )
{
   boolValue = _value;
}
