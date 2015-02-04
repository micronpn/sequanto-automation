/* To try this example use the menu item:
 *
 *     Tools->Generate Automation
 *
 * Before compiling.
 *
 * BEGIN AUTOMATION
 *
 * function /hello hello_world
 *
 * END AUTOMATION
 */

char * hello_world ( const char * _name )
{
    char * ret = (char*) malloc ( 20 );
    sprintf ( ret, "Hello %s!", _name );
    return ret;
}

void setup ()
{
    SequantoAutomation::init();
}

void loop ()
{
    SequantoAutomation::poll();
}
