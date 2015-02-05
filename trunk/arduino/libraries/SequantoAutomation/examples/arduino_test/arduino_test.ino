/* To try this example use the menu item:
 *
 *     Tools->Generate Automation
 *
 * Before compiling.
 *
 * BEGIN AUTOMATION
 *
 * name arduino_test
 * import arduino_test.c
 * property /sequanto_automation_version sequanto_automation_version
 * property /digital/(2..13)/mode digital_mode_get digital_mode_set
 * property /digital/(2..13)/counter digital_counter_get digital_counter_set
 * property /digital/(2..13)/pin digital_pin_get digital_pin_set
 *
 * # property /digital/(0..14)/mode digital_mode_get digital_mode_set
 * # property /digital/(0..14)/counter digital_counter_get digital_counter_set
 * # property /digital/(0..14)/pin digital_pin_get digital_pin_set
 *
 * END AUTOMATION */

/* Skip the first two pins, they are used by the USB port for communication. */
#define PIN_OFFSET 2
#define NUMBER_OF_PINS 14

static char buff[10];

const char * sequanto_automation_version ( void )
{
    sq_get_constant_string_copy ( SQ_STRING_CONSTANT(SQ_SVN_REVISION), buff );
    return buff;
}

typedef struct _DigitalPin
{
    enum {
        DIGITAL_PIN_INPUT,
        DIGITAL_PIN_OUTPUT,
        DIGITAL_PIN_TIMER,
    } m_type;

    int m_counter;
    SQBool m_pin;
} DigitalPin;

static DigitalPin s_pins[NUMBER_OF_PINS - PIN_OFFSET];

static const char DIGITAL_PIN_INPUT_TEXT[] SQ_CONST_VARIABLE = "Input";
static const char DIGITAL_PIN_OUTPUT_TEXT[] SQ_CONST_VARIABLE = "Output";
static const char DIGITAL_PIN_TIMER_TEXT[] SQ_CONST_VARIABLE = "Timer";
static const char DIGITAL_PIN_UNKNOWN_TEXT[] SQ_CONST_VARIABLE = "Unknown";

const char * digital_mode_get ( int _pin )
{
    switch ( s_pins[_pin - PIN_OFFSET].m_type )
    {
    case DigitalPin::DIGITAL_PIN_INPUT:
        sq_get_constant_string_copy(DIGITAL_PIN_INPUT_TEXT, buff);
        break;

    case DigitalPin::DIGITAL_PIN_OUTPUT:
        sq_get_constant_string_copy(DIGITAL_PIN_OUTPUT_TEXT, buff);
        break;

    case DigitalPin::DIGITAL_PIN_TIMER:
        sq_get_constant_string_copy(DIGITAL_PIN_TIMER_TEXT, buff);
        break;

    default:
        sq_get_constant_string_copy(DIGITAL_PIN_UNKNOWN_TEXT, buff);
        break;
    }
    return buff;
}

void digital_mode_set ( int _pin, const char * _mode )
{
    if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_INPUT_TEXT ) == 0 )
    {
        s_pins[_pin - PIN_OFFSET].m_type = DigitalPin::DIGITAL_PIN_INPUT;
    }
    else if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_OUTPUT_TEXT ) == 0 )
    {
        s_pins[_pin - PIN_OFFSET].m_type = DigitalPin::DIGITAL_PIN_OUTPUT;
    }
    else if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_TIMER_TEXT ) == 0 )
    {
        s_pins[_pin - PIN_OFFSET].m_type = DigitalPin::DIGITAL_PIN_TIMER;
    }
    else
    {
        //sq_logf ( "Trying to set illegal mode: %s", _mode );
        s_pins[_pin - PIN_OFFSET].m_type = DigitalPin::DIGITAL_PIN_OUTPUT;
        sq_digital_mode_updated ( _pin, digital_mode_get(_pin) );
    }

    switch ( s_pins[_pin - PIN_OFFSET].m_type )
    {
    case DigitalPin::DIGITAL_PIN_INPUT:
        pinMode ( _pin, INPUT );
        /* Set pull-up resistor. */
        digitalWrite ( _pin, HIGH );
        s_pins[_pin - PIN_OFFSET].m_pin = digitalRead ( _pin ) == HIGH;
        sq_digital_pin_updated ( _pin, s_pins[_pin].m_pin );
        break;

    case DigitalPin::DIGITAL_PIN_OUTPUT:
        pinMode ( _pin, OUTPUT );
        digitalWrite ( _pin, LOW );
        break;

    case DigitalPin::DIGITAL_PIN_TIMER:
        pinMode ( _pin, INPUT );
        /* Set pull-up resistor. */
        digitalWrite ( _pin, HIGH );
        s_pins[_pin - PIN_OFFSET].m_counter = 0;
        s_pins[_pin - PIN_OFFSET].m_pin = digitalRead ( _pin ) == HIGH;
        sq_digital_counter_updated ( _pin, 0 );
        break;
    }
}

int digital_counter_get ( int _pin )
{
    return s_pins[_pin - PIN_OFFSET].m_counter;
}

void digital_counter_set ( int _pin, int _counter )
{
    s_pins[_pin - PIN_OFFSET].m_counter = _counter;
}

SQBool digital_pin_get ( int _pin )
{
    return s_pins[_pin - PIN_OFFSET].m_pin;
}

void digital_pin_set ( int _pin, SQBool _value )
{
    if ( s_pins[_pin - PIN_OFFSET].m_type == DigitalPin::DIGITAL_PIN_OUTPUT )
    {
        digitalWrite ( _pin, _value == SQ_TRUE ? HIGH : LOW );
    }
    else
    {
        //sq_logf ( "Trying to set pin %i, which is not an output pin.", _pin );
    }
}

void setup ( void )
{
    int i;
    for ( i = PIN_OFFSET; i < NUMBER_OF_PINS; i ++ )
    {
        pinMode ( i, OUTPUT );
        s_pins[i - PIN_OFFSET].m_type = DigitalPin::DIGITAL_PIN_OUTPUT;
        s_pins[i - PIN_OFFSET].m_counter = 0;
    }
    SequantoAutomation::init();
}

void loop ( void )
{
    int i;
    SQBool newValue;

    SequantoAutomation::poll();

    for ( i = PIN_OFFSET; i < NUMBER_OF_PINS; i ++ )
    {
        switch ( s_pins[i - PIN_OFFSET].m_type )
        {
        case DigitalPin::DIGITAL_PIN_TIMER:
            newValue = digitalRead ( i ) == HIGH;
            if ( s_pins[i - PIN_OFFSET].m_pin != newValue )
            {
                s_pins[i - PIN_OFFSET].m_pin = newValue;

                /* If we go from high to low, the change should be counted. */
                if ( s_pins[i - PIN_OFFSET].m_pin == SQ_FALSE )
                {
                    s_pins[i - PIN_OFFSET].m_counter += 1;
                }
            }
            break;

        case DigitalPin::DIGITAL_PIN_INPUT:
            newValue = digitalRead ( i ) == HIGH;
            if ( s_pins[i - PIN_OFFSET].m_pin != newValue )
            {
                s_pins[i - PIN_OFFSET].m_pin = newValue;

                sq_digital_pin_updated ( i, newValue );
            }
            break;

        default:
            break;
        }
    }
}
