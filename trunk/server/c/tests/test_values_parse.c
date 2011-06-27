#include "sequanto_test.h"

#include <sequanto/value.h>

#define NUM_VALUES 10
static SQValue values[NUM_VALUES];
static void init ()
{
    int i;
    for ( i = 0; i < NUM_VALUES; i++ )
    {
        sq_value_init ( &values[i] );
    }
}

static void done()
{
    int i;
    for ( i = 0; i < NUM_VALUES; i++ )
    {
        sq_value_free ( &values[i] );
    }
}

static void expect_empty ( int _skipFirst )
{
    int i;
    for ( i = _skipFirst; i < NUM_VALUES; i++ )
    {
        fail_unless ( values[i].m_type == VALUE_TYPE_NO_VALUE );
    }
}

START_TEST(test_single_integer)
{
    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "42\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_INTEGER );
    fail_unless ( values[0].Value.m_integerValue == 42 );
    
    expect_empty ( 1 );
    
    done();
}
END_TEST

START_TEST(test_single_string)
{
    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "\"Rasmus Toftdahl Olesen\"\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_STRING );
    ck_assert_str_eq( values[0].Value.m_stringValue, "Rasmus Toftdahl Olesen" );
    
    expect_empty ( 1 );
    
    done();

    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "\"Foo\\n\\\"Bar\"\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_STRING );
    ck_assert_str_eq( values[0].Value.m_stringValue, "Foo\n\"Bar" );
    
    expect_empty ( 1 );
    
    done();
}
END_TEST

START_TEST(test_single_float)
{
    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "3.14\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_FLOAT );
    
    ck_assert_float_eq ( values[0].Value.m_floatValue, 3.14 );
    
    expect_empty ( 1 );
    
    done();
}
END_TEST

START_TEST(test_single_boolean)
{
    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "true\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_BOOLEAN );
    fail_unless ( values[0].Value.m_booleanValue == SQ_TRUE );
    
    expect_empty ( 1 );
    
    done();
}
END_TEST

START_TEST(test_single_byte_array)
{
    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "0xDEAD\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_BYTE_ARRAY );
    fail_unless ( values[0].Value.m_byteArrayValue->m_length == 2 );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[0] == 0xDE );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[1] == 0xAD );
    
    expect_empty ( 1 );
    
    done();

    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "0xdeaf2243\r\n" ) == 1 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_BYTE_ARRAY );
    fail_unless ( values[0].Value.m_byteArrayValue->m_length == 4 );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[0] == 0xDE );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[1] == 0xAF );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[2] == 0x22 );
    fail_unless ( values[0].Value.m_byteArrayValue->m_start[3] == 0x43 );
    
    expect_empty ( 1 );
    
    done();

    init();
    
    fail_unless ( sq_values_parse ( values, NUM_VALUES, (SQByte*) "0xdead2243f\r\n" ) == 0 );
    
    fail_unless ( values[0].m_type == VALUE_TYPE_NO_VALUE );
    
    expect_empty ( 0 );
    
    done();
}
END_TEST

SQ_TEST_SUITE(values_parse_suite, "sq_values_parse");
SQ_TEST_CASE(test_single_integer);
SQ_TEST_CASE(test_single_string);
SQ_TEST_CASE(test_single_float);
SQ_TEST_CASE(test_single_boolean);
SQ_TEST_CASE(test_single_byte_array);
SQ_END_SUITE();
