#include <check.h>
#include "sequanto_test.h"

SQ_TEST_SUITE_MAIN(value_suite);
SQ_TEST_SUITE_MAIN(server_suite);
SQ_TEST_SUITE_MAIN(parser_suite);
SQ_TEST_SUITE_MAIN(circularbuffer_suite);
SQ_TEST_SUITE_MAIN(values_parse_suite);

int main ( int _argc, char * _argv[] )
{
    int number_failed;
    SRunner *sr = srunner_create ( value_suite() );
    srunner_add_suite ( sr, parser_suite() );
    srunner_add_suite ( sr, values_parse_suite() );
    srunner_add_suite ( sr, circularbuffer_suite() );
    srunner_add_suite ( sr, server_suite() );
    srunner_set_xml ( sr, "output.xml" );
    if ( _argc > 1 && strcmp(_argv[1], "--no-fork") == 0 )
    {
        srunner_set_fork_status ( sr, CK_NOFORK );
    }
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? 0 : -1;
}
