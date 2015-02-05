/* To try this example use the menu item:
 *
 *     Tools->Generate Automation
 *
 * Before compiling.
 *
 * BEGIN AUTOMATION
 *
 * function /hello hello_world
 * property /led led_get led_set
 *
 * END AUTOMATION
 */

static SQBool s_led = SQ_FALSE;

SQBool led_get ()
{
  return s_led;
}

void led_get ( SQBool v )
{
  s_led = v;
}

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
