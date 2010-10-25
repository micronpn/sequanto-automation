#include "sequanto_test.h"

#include <sequanto/parser.h>
#include <sequanto/stream.h>
#include <sequanto/server.h>

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

START_TEST(test_parser_bad_command)
{
    // We need a server in order for !LOG commands to work.
    SQServer server;
    sq_server_init ( &server, -1 );
    
    test_stream = server.m_stream; //sq_stream_open ( -1 );
    
    input_string ( &server.m_parser, "Ostemad\r\n" );

    char * output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "!LOG \"Bad command 'Ostemad\\r'\"\r\n-\"Unrecognized command.\"\r\n" );
    
    free ( output );
    
    sq_server_destroy ( &server );
}
END_TEST

START_TEST(test_parser_get_property)
{
    SQParser parser;

    // Setup
    
    test_stream = sq_stream_open ( -1 );
    
    sq_parser_init ( &parser );
    
    
    // GET a property with positive result
    input_string ( &parser, "GET /this/is/a/property\r\n" );

    char * output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "+12\r\n" );
    
    free ( output );
    
    
    // GET a property with negative result
    input_string ( &parser, "GET /this/is/NOT_a/property\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "- \"Unknown object!\"\r\n" );
    
    free ( output );

    
    // GET a property with parameters (not possible)
    
    input_string ( &parser, "GET /this/is/a/property 42 \"Ostemad\"\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "-\"GET request must be called without values\"\r\n" );
    
    free ( output );
    
    
    // Cleanup
    
    sq_stream_close ( test_stream );
    sq_parser_destroy ( &parser );
}
END_TEST


START_TEST(test_parser_set_property)
{
    SQParser parser;
    
    // Setup
    
    test_stream = sq_stream_open ( -1 );
    
    sq_parser_init ( &parser );
    
    
    // SET an existing property
    input_string ( &parser, "SET /My_Integer_Property 34\r\n" );

    char * output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "+\r\n" );
    
    free ( output );

    
    // SET a non existing property
    input_string ( &parser, "SET /UnknownIntegerProperty 12\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "- \"Unknown object!\"\r\n" );
    
    free ( output );


    // SET a property without providing value
    input_string ( &parser, "SET /My_Integer_Property\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "-\"SET request must be called with one value\"\r\n" );
    
    free ( output );

    
    // SET a property while providing too many values value
    input_string ( &parser, "SET /My_Integer_Property 1 2 3 4 5 0xDEADBEAF\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "-\"SET request must be called with one value\"\r\n" );
    
    free ( output );
    
    
    // Cleanup
    
    sq_stream_close ( test_stream );
    sq_parser_destroy ( &parser );
}
END_TEST


START_TEST(test_parser_call)
{
    SQParser parser;
    
    // Setup
    
    test_stream = sq_stream_open ( -1 );
    
    sq_parser_init ( &parser );
    
    
    // CALL function
    input_string ( &parser, "CALL /my/function\r\n" );

    char * output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "+ 42\r\n" );
    
    free ( output );

    
    // CALL a non existing property
    input_string ( &parser, "CALL /no_function\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "- \"Unknown object!\"\r\n" );
    
    free ( output );


    // CALL function with too many values
    input_string ( &parser, "CALL /my/function 0 1 2 3 4 5 5 6 7 8 9 10 11 12\r\n" );
    
    output = sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "-\"Too many values given in CALL request\"\r\n" );
    
    free ( output );
    
    
    // Cleanup
    
    sq_stream_close ( test_stream );
    sq_parser_destroy ( &parser );
}
END_TEST


SQ_TEST_SUITE(parser_suite, "SQParser");
SQ_TEST_CASE(test_parser_init);
SQ_TEST_CASE(test_parser_input_byte);
SQ_TEST_CASE(test_parser_bad_command);
SQ_TEST_CASE(test_parser_get_property);
SQ_TEST_CASE(test_parser_set_property);
SQ_TEST_CASE(test_parser_call);
SQ_END_SUITE();
