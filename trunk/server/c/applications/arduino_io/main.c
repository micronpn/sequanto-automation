#include <sequanto/automation.h>
#include <avr/io.h>

#include "arduino_io_automation.h"

#include "wiring.h"

/* Skip the first two pins, they are used by the USB port for communication. */
#define PIN_OFFSET 2
#define NUMBER_OF_PINS 14
#define IO_MASK 0x3FFC

static char buff[10];

const char * sequanto_automation_version ( void )
{
   sq_get_constant_string_copy ( SQ_STRING_CONSTANT(SVN_REVISION), buff );
   return buff;
}

enum DIGITAL_PIN_DIRECTION {
	DIGITAL_PIN_INPUT,
	DIGITAL_PIN_OUTPUT
};

static int s_counters[NUMBER_OF_PINS];


static unsigned int io_direction = 0;	// All input on startup; for safety reasons.
static unsigned int io_last_read = IO_MASK;	// Pull-up on all; all high on startup.
static unsigned int io_last_written = 0;	// Pull-up on all; all high on startup.
static unsigned int io_counter_enable = 0;	// No counters enabled
static unsigned int io_trig_h2l = IO_MASK;	// default counter edge
static unsigned int io_trig_l2h = 0;

static const char DIGITAL_PIN_INPUT_TEXT[] SQ_CONST_VARIABLE = "Input";
static const char DIGITAL_PIN_OUTPUT_TEXT[] SQ_CONST_VARIABLE = "Output";

void update_direction_mask( void )
{
	io_direction = (DDRD | (((unsigned int)DDRB) << 8 )) & IO_MASK;
}

#define PIN_MASK(pin) (((unsigned int)1) << pin)

SQBool digital_pin_get ( int _pin )
{
   if ((io_direction& PIN_MASK(_pin)) != 0)
   {
      return ((io_last_written & PIN_MASK(_pin)) != 0);
   }
   else
   {
      return ((io_last_read & PIN_MASK(_pin)) != 0);
   }
}

void digital_pin_set ( int _pin, SQBool _value )
{
   //if ((io_direction& PIN_MASK(_pin)) != 0)
   {
      digitalWrite ( _pin, _value == SQ_TRUE ? HIGH : LOW );
      io_last_written = (io_last_written & (~(PIN_MASK(_pin)))) | (_value >> _pin);
      sq_digital_pin_updated ( _pin, _value );
   }
   /*
   else
   {
      sq_logf ( "Trying to set pin %i, which is not an output pin.", _pin );
   }
   */
}

const char * digital_direction_get ( int _pin )
{
	update_direction_mask();
	if ( io_direction & PIN_MASK(_pin) )
	{
      sq_get_constant_string_copy(DIGITAL_PIN_OUTPUT_TEXT, buff);
	}
	else
	{
      sq_get_constant_string_copy(DIGITAL_PIN_INPUT_TEXT, buff);
	}
   return buff;
}

void digital_direction_set ( int _pin, const char * _mode )
{
	enum DIGITAL_PIN_DIRECTION dir = DIGITAL_PIN_INPUT;
   if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_INPUT_TEXT ) == 0 )
   {
      dir = DIGITAL_PIN_INPUT;
   }
   else if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_OUTPUT_TEXT ) == 0 )
   {
      dir = DIGITAL_PIN_OUTPUT;
   }
   else
   {
      sq_logf ( "Trying to set illegal mode: %s", _mode );
      return;
   }
   
   switch ( dir )
   {
   case DIGITAL_PIN_INPUT:
      pinMode ( _pin, INPUT );
      /* Set pull-up resistor. */
      digitalWrite ( _pin, HIGH );
      sq_digital_pin_updated ( _pin, digital_pin_get(_pin) );
      break;

   case DIGITAL_PIN_OUTPUT:
      pinMode ( _pin, OUTPUT );
      digitalWrite ( _pin, LOW );
      break;
   }
	update_direction_mask();
}

int digital_counter_get ( int _pin )
{
   return s_counters[_pin];
}

void digital_counter_set ( int _pin, int _counter )
{
   s_counters[_pin] = _counter;
}

SQBool digital_counter_enabled_get ( int _pin )
{
   return ((io_counter_enable & PIN_MASK(_pin)) != 0) ? SQ_TRUE : SQ_FALSE;
}

void digital_counter_enabled_set ( int _pin, SQBool _value )
{
	unsigned int mask = PIN_MASK(_pin);
	io_counter_enable = (io_counter_enable & ~mask) | ((_value == SQ_TRUE) ? mask : 0);
}

SQBool digital_counter_high2low_get ( int _pin )
{
   return ((io_trig_h2l & PIN_MASK(_pin)) != 0) ? SQ_TRUE : SQ_FALSE;
}

void digital_counter_high2low_set ( int _pin, SQBool _value )
{
	unsigned int mask = PIN_MASK(_pin);
	io_trig_h2l= (io_trig_h2l & ~mask) | ((_value == SQ_TRUE) ? mask : 0);
}

SQBool digital_counter_low2high_get ( int _pin )
{
   return ((io_trig_l2h & PIN_MASK(_pin)) != 0) ? SQ_TRUE : SQ_FALSE;
}

void digital_counter_low2high_set ( int _pin, SQBool _value )
{
	unsigned int mask = PIN_MASK(_pin);
	io_trig_l2h = (io_trig_l2h & ~mask) | ((_value == SQ_TRUE) ? mask : 0);
}

int analog_in_get_value ( int _pin )
{
   unsigned int ret = analogRead(_pin);
   ret *= 49;
   ret += 5;
   ret /= 10;
   return (int) ret;
}

int analog_in_get_raw_value ( int _pin )
{
   return analogRead(_pin);
}

void analog_in_use_default_reference ( void  )
{
   analogReference ( DEFAULT );
}

void analog_in_use_external_reference ( void  )
{
   analogReference ( EXTERNAL );
}

int main ( void )
{
   int i;
   for ( i = PIN_OFFSET; i < NUMBER_OF_PINS; i++ )
   {
      pinMode ( i, INPUT);
		digitalWrite ( i, HIGH );	// pull-up
		s_counters[i] = 0;
   }
   
   static SQServer server;
   
   sq_init ();   
   
   sq_server_init ( &server, 4321 );

	unsigned int inputs = PIND | (((unsigned int)PINB) << 8 );
	unsigned int changed, trigged = 0;
   
   while ( SQ_TRUE )
   {
      sq_server_poll ( &server );

		inputs = (PIND | (((unsigned int)PINB) << 8 )) & (~io_direction);
		changed = inputs ^ io_last_read;
		trigged = changed & io_counter_enable & (((io_trig_h2l ^ inputs ) & io_trig_h2l) | (((~io_trig_l2h) ^ inputs ) & io_trig_l2h));
		io_last_read = inputs;

		for ( i = PIN_OFFSET ; i < NUMBER_OF_PINS ; i++ )
		{
			if ( ( changed & PIN_MASK(i) ) != 0 )
			{
				sq_digital_pin_updated ( i, digital_pin_get(i) );
			}
			if ( ( trigged & PIN_MASK(i) ) != 0 )
			{
				s_counters[i]++;
			}
		}
   }
   
   sq_shutdown ();   
}
