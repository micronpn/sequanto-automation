#include "sequanto_test.h"

#include <sequanto/parser.h>
#include <sequanto/stream.h>

static SQStream * test_stream;

START_TEST(test_parser_init)
{
    SQParser parser;
    
    sq_parser_init ( &parser );
    
    fail_unless ( parser.m_inputBufferPosition == 0 );
    
    sq_parser_destroy ( &parser );
}
END_TEST

static void input_string ( SQParser * _parser, const char * _bytes )
{
    for ( ; *_bytes != '\0'; _bytes++ )
    {
        sq_parser_input_byte ( _parser, test_stream, *_bytes );
    }
}

START_TEST(test_parser_input_byte)
{
    SQParser parser;
    
    test_stream = sq_stream_open ( -1 );
    
    sq_parser_init ( &parser );
    
    fail_unless ( parser.m_inputBufferPosition == 0 );
    
    sq_parser_input_byte ( &parser, test_stream, 'P' );
    
    fail_unless ( parser.m_inputBuffer[0] == 'P' );
    fail_unless ( parser.m_inputBufferPosition == 1 );
    
    input_string ( &parser, "ROTOCOL" );

    ck_assert_int_eq ( parser.m_inputBufferPosition, 8 );
    
    input_string ( &parser, "\r\n" );

    ck_assert_int_eq ( parser.m_inputBufferPosition, 0 );
 
    char * output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "+PROTOCOL 1\r\n" );
    
    free ( output );
    
    sq_stream_close ( test_stream );
    sq_parser_destroy ( &parser );
}
END_TEST

SQ_TEST_SUITE(parser_suite, "SQParser");
SQ_TEST_CASE(test_parser_init);
SQ_TEST_CASE(test_parser_input_byte);
SQ_END_SUITE();
