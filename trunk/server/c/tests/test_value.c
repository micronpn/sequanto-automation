#include "sequanto_test.h"

#include <sequanto/value.h>

START_TEST(test_value_init)
{
    SQValue value;
    sq_value_init ( &value );

    fail_unless ( value.m_type == VALUE_TYPE_NO_VALUE );
    /*
    fail_unless ( value.Value.m_integerValue == 0 );
    fail_unless ( value.Value.m_floatValue == 0.0f );
    fail_unless ( value.Value.m_booleanValue == SQ_FALSE );
    fail_unless ( value.Value.m_stringValue == NULL );
    fail_unless ( value.Value.m_constStringValue == NULL );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue == NULL );
    fail_unless ( value.Value.ArrayValue.m_byteArrayLength == 0 );
    */
}
END_TEST

START_TEST(test_value_integer)
{
    SQValue value;
    sq_value_init ( &value );
    
    sq_value_integer ( &value, 42 );
    
    fail_unless ( value.m_type == VALUE_TYPE_INTEGER );
    fail_unless ( value.Value.m_integerValue == 42 );
}
END_TEST

START_TEST(test_value_boolean)
{
    SQValue value;
    sq_value_init ( &value );
    
    sq_value_boolean ( &value, SQ_TRUE );
    
    fail_unless ( value.m_type == VALUE_TYPE_BOOLEAN );
    fail_unless ( value.Value.m_booleanValue == SQ_TRUE );
}
END_TEST

START_TEST(test_value_float)
{
    SQValue value;
    sq_value_init ( &value );
    
    sq_value_float ( &value, -3.14 );
    
    fail_unless ( value.m_type == VALUE_TYPE_FLOAT );
    fail_unless ( value.Value.m_floatValue == -3.14f );
}
END_TEST

START_TEST(test_value_string)
{
    SQValue value;
    sq_value_init ( &value );
    
    char * name = strdup("Rasmus Toftdahl Olesen");
    
    sq_value_string ( &value, name );
    
    fail_unless ( value.m_type == VALUE_TYPE_STRING );
    ck_assert_str_eq ( value.Value.m_stringValue, name );
    fail_unless ( ((void*) value.Value.m_stringValue) == ((void*) name) );
    
    sq_value_free ( &value );
    
    fail_unless ( value.m_type == VALUE_TYPE_NO_VALUE );
    fail_unless ( value.Value.m_stringValue == NULL );
}
END_TEST

START_TEST(test_value_string_copy)
{
    SQValue value;
    sq_value_init ( &value );
    
    char * name = strdup("Rasmus Toftdahl Olesen");
    
    sq_value_string_copy ( &value, name );
    
    fail_unless ( value.m_type == VALUE_TYPE_STRING );
    ck_assert_str_eq ( value.Value.m_stringValue, name );
    fail_unless ( ((void*) value.Value.m_stringValue) != ((void*) name) );
    
    sq_value_free ( &value );
    
    fail_unless ( value.m_type == VALUE_TYPE_NO_VALUE );
    fail_unless ( value.Value.m_stringValue == NULL );
    
    free(name);
}
END_TEST

START_TEST(test_value_const_string)
{
    SQValue value;
    sq_value_init ( &value );
    
    const char name[] = "Rasmus Toftdahl Olesen";
    
    sq_value_const_string ( &value, name );
    
    fail_unless ( value.m_type == VALUE_TYPE_CONST_STRING );
    ck_assert_str_eq ( value.Value.m_stringValue, name );
    fail_unless ( ((void*) value.Value.m_stringValue) == ((void*) &name) );
    
    sq_value_free ( &value );
    
    fail_unless ( value.m_type == VALUE_TYPE_NO_VALUE );
    fail_unless ( value.Value.m_stringValue == NULL );
    
    ck_assert_str_eq ( "Rasmus Toftdahl Olesen", name );
}
END_TEST

START_TEST(test_value_null)
{
    SQValue value;
    sq_value_init ( &value );
    
    sq_value_null ( &value );
    
    fail_unless ( value.m_type == VALUE_TYPE_NULL );
    
    sq_value_free ( &value );
}
END_TEST

START_TEST(test_value_byte_array)
{
    SQValue value;
    sq_value_init ( &value );
    
    int length = 4;
    SQByte *values = malloc ( sizeof(SQByte) * length );
    values[0] = 0xAA;
    values[1] = 0xBB;
    values[2] = 0xCC;
    values[3] = 0xDD;
    
    sq_value_byte_array ( &value, values, length );
    
    fail_unless ( value.m_type == VALUE_TYPE_BYTE_ARRAY );
    fail_unless ( value.Value.ArrayValue.m_byteArrayLength == length );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue[0] == 0xAA );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue[1] == 0xBB );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue[2] == 0xCC );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue[3] == 0xDD );
    
    sq_value_free ( &value );
    
    fail_unless ( value.m_type == VALUE_TYPE_NO_VALUE );
    fail_unless ( value.Value.ArrayValue.m_byteArrayLength == 0 );
    fail_unless ( value.Value.ArrayValue.m_byteArrayValue == NULL );
}
END_TEST

SQ_TEST_SUITE(value_suite, "SQValue");
SQ_TEST_CASE(test_value_init);
SQ_TEST_CASE(test_value_integer);
SQ_TEST_CASE(test_value_boolean);
SQ_TEST_CASE(test_value_float);
SQ_TEST_CASE(test_value_string);
SQ_TEST_CASE(test_value_string_copy);
SQ_TEST_CASE(test_value_const_string);
SQ_TEST_CASE(test_value_byte_array);
SQ_TEST_CASE(test_value_null);
SQ_END_SUITE();
