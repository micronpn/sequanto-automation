#include <check.h>

int main ( int _argc, char * _argv[] )
{
    int number_failed;
    SRunner *sr = srunner_create ( (Suite*) value_suite() );
    srunner_add_suite ( sr, (Suite*) parser_suite() );
    srunner_add_suite ( sr, (Suite*) values_parse_suite() );
    srunner_add_suite ( sr, (Suite*) circularbuffer_suite() );
    srunner_add_suite ( sr, (Suite*) server_suite() );
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
