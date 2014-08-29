#include "sequanto_test.h"

#include <sequanto/circularbuffer.h>

#define BUFFER_SIZE 1024

SQ_CIRCULAR_BUFFER_DEFINE(buffer, BUFFER_SIZE);

START_TEST(test_circularbuffer_init)
{
    SQ_CIRCULAR_BUFFER_INIT(buffer, BUFFER_SIZE);
    
    fail_unless ( SQ_CIRCULAR_BUFFER_AVAILABLE(buffer) == 0 );
    fail_unless ( buffer.m_size == BUFFER_SIZE );
    fail_unless ( buffer.m_readPosition == 0 );
    fail_unless ( buffer.m_writePosition == 0 );
}
END_TEST

START_TEST(test_circularbuffer_push_and_pop)
{
    SQ_CIRCULAR_BUFFER_INIT(buffer, BUFFER_SIZE);
    
    SQ_CIRCULAR_BUFFER_PUSH(buffer, 0x1f);
    
    fail_unless ( buffer.m_readPosition == 0 );
    fail_unless ( buffer.m_writePosition == 1 );
    
    fail_unless ( SQ_CIRCULAR_BUFFER_AVAILABLE(buffer) == 1 );

    fail_unless ( SQ_CIRCULAR_BUFFER_POP(buffer) == 0x1f );
    fail_unless ( buffer.m_readPosition == 1 );
    fail_unless ( buffer.m_writePosition == 1 );
    
    fail_unless ( SQ_CIRCULAR_BUFFER_AVAILABLE(buffer) == 0 );
}
END_TEST

static void fill_buffer()
{
    int i;
    
    for ( i = 0; i < BUFFER_SIZE; i++ )
    {
        SQ_CIRCULAR_BUFFER_PUSH(buffer, (SQByte) i );
    }
}

START_TEST(test_circularbuffer_push_until_full)
{
    SQ_CIRCULAR_BUFFER_INIT(buffer, BUFFER_SIZE);
    
    fill_buffer();
    
    ck_assert_int_eq(SQ_CIRCULAR_BUFFER_AVAILABLE(buffer), 1024 );
}
END_TEST

START_TEST(test_circularbuffer_pop_when_full)
{
    int i;

    SQ_CIRCULAR_BUFFER_INIT(buffer, BUFFER_SIZE);
    
    fill_buffer();
    
    for ( i = 0; i < BUFFER_SIZE; i++ )
    {
        ck_assert_int_eq(SQ_CIRCULAR_BUFFER_AVAILABLE(buffer), BUFFER_SIZE - i );
        
        fail_unless ( SQ_CIRCULAR_BUFFER_POP(buffer ) == ((SQByte) i) );
    }
    ck_assert_int_eq(SQ_CIRCULAR_BUFFER_AVAILABLE(buffer), 0 );
}
END_TEST

SQ_TEST_SUITE(circularbuffer_suite, "sq_circularbuffer");
SQ_TEST_CASE(test_circularbuffer_init);
SQ_TEST_CASE(test_circularbuffer_push_and_pop);
SQ_TEST_CASE(test_circularbuffer_push_until_full);
SQ_TEST_CASE(test_circularbuffer_pop_when_full);
SQ_END_SUITE();
