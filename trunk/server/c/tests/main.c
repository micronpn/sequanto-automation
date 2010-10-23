#include <check.h>

int main ( int _argc, char * argv[] )
{
    int number_failed;
    SRunner *sr = srunner_create ( (Suite*) value_suite() );
    srunner_add_suite ( sr, (Suite*) parser_suite() );
    srunner_add_suite ( sr, (Suite*) values_parse_suite() );
    srunner_set_xml ( sr, "output.xml" );
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? 0 : -1;
}
