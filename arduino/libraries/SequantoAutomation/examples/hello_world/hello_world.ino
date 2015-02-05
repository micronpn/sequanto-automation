/* To try this example use the menu item:
 *
 *     Tools->Generate Automation
 *
 * Before compiling.
 *
 * BEGIN AUTOMATION
 *
 * function /mult multiply
 * property /led led_get led_set
 * property /iterations iterations_get
 *
 * END AUTOMATION
 */

const int LED = 13;

int multiply(int a, int b)
{
  return a*b;
}

static SQBool s_led = SQ_FALSE;

SQBool led_get ( void )
{
  return s_led;
}

void led_set ( SQBool v )
{
  s_led = v;
  digitalWrite ( LED, s_led == SQ_TRUE ? HIGH : LOW );
}

static int s_iterations = 0;

int iterations_get ( void )
{
  return s_iterations;
}

void setup ()
{
    SequantoAutomation::init();
    pinMode ( LED, OUTPUT );
    s_led = SQ_FALSE;
    digitalWrite ( LED, LOW );
    s_iterations = 0;
}

void loop ()
{
    s_iterations ++;
    SequantoAutomation::poll();
}
