#include "sequanto_test.h"

#include <sequanto/server.h>

void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _inputValues, int _numberOfValues )
{
}

void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
}

void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _value )
{
}

void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
}

void sq_parser_list ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
}

void sq_parser_enable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
}

void sq_parser_disable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
}

START_TEST(test_server_init)
{
    SQServer server;
    
    sq_server_init ( &server, -1 );
}
END_TEST

SQ_TEST_SUITE(server_suite, "SQServer");
SQ_TEST_CASE(test_server_init);
SQ_END_SUITE();
