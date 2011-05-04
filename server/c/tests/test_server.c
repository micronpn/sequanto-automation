#include "sequanto_test.h"

#include <sequanto/server.h>

void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _inputValues, int _numberOfValues )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_inputValues);
    SQ_UNUSED_PARAMETER(_numberOfValues);
    
    if ( strcmp(_objectPath, "/my/function") == 0 )
    {
        sq_stream_write_string ( _stream, "+ 42\r\n" );
    }
    else
    {
        sq_stream_write_string ( _stream, "- \"Unknown object!\"\r\n" );
    }
}

void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
    SQ_UNUSED_PARAMETER(_parser);
    
    if ( strcmp(_objectPath, "/this/is/a/property") == 0 )
    {
        sq_stream_write_string ( _stream, "+12\r\n" );
    }
    else
    {
        sq_stream_write_string ( _stream, "- \"Unknown object!\"\r\n" );
    }
}

void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _value )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_value);
    
    if ( strcmp(_objectPath, "/My_Integer_Property") == 0 )
    {
        sq_stream_write_string ( _stream, "+\r\n" );
    }
    else
    {
        sq_stream_write_string ( _stream, "- \"Unknown object!\"\r\n" );
    }
}

void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_stream);
    SQ_UNUSED_PARAMETER(_objectPath);
}

void sq_parser_list ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_stream);
    SQ_UNUSED_PARAMETER(_objectPath);
}

void sq_parser_enable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_stream);
    SQ_UNUSED_PARAMETER(_objectPath);
}

void sq_parser_disable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
    SQ_UNUSED_PARAMETER(_parser);
    SQ_UNUSED_PARAMETER(_stream);
    SQ_UNUSED_PARAMETER(_objectPath);
}

START_TEST(test_server_init)
{
    SQServer server;
    
    sq_server_init ( &server, -1 );

    sq_server_destroy ( &server );
}
END_TEST

SQ_TEST_SUITE(server_suite, "SQServer");
SQ_TEST_CASE(test_server_init);
SQ_END_SUITE();
