#include "sequanto_test.h"

#include <sequanto/parser.h>
#include <sequanto/stream.h>
#include <sequanto/server.h>
#include <sequanto/protocol.h>

static SQStream * test_stream;

START_TEST(test_protocol_send_add)
{
    test_stream = sq_stream_open ( -1 );
    
    sq_protocol_write_add_message ( test_stream, "/this/is/my/object" );
    
    char * output = (char*) sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "!ADD /this/is/my/object\r\n" );
    
    free ( output );
    
    sq_stream_close ( test_stream );
}
END_TEST

START_TEST(test_protocol_send_remove)
{
    test_stream = sq_stream_open ( -1 );
    
    sq_protocol_write_remove_message ( test_stream, "/this/is/my/object" );
    
    char * output = (char*) sq_stream_unit_test_pop_write ( test_stream );
    
    ck_assert_str_eq ( output, "!REMOVE /this/is/my/object\r\n" );
    
    free ( output );
    
    sq_stream_close ( test_stream );
}
END_TEST


SQ_TEST_SUITE(protocol_suite, "SQProtocol");
SQ_TEST_CASE(test_protocol_send_add);
SQ_TEST_CASE(test_protocol_send_remove);
SQ_END_SUITE();
