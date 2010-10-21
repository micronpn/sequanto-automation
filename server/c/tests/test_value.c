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

SQ_TEST_SUITE(value_suite, "SQValue");
SQ_TEST_CASE(test_value_init);
SQ_TEST_CASE(test_value_integer);
SQ_END_SUITE();
