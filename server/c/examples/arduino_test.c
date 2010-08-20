#include <sequanto/automation.h>

#include "arduino_test_automation.h"

#include "wiring.h"

#define NUMBER_OF_PINS 12

const char * sequanto_automation_version ( void )
{
   return sq_get_constant_string(SQ_STRING_CONSTANT(SVN_REVISION));
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

static DigitalPin s_pins[NUMBER_OF_PINS];

static const char DIGITAL_PIN_INPUT_TEXT[] SQ_CONST_VARIABLE = "Input";
static const char DIGITAL_PIN_OUTPUT_TEXT[] SQ_CONST_VARIABLE = "Output";
static const char DIGITAL_PIN_TIMER_TEXT[] SQ_CONST_VARIABLE = "Timer";
static const char DIGITAL_PIN_UNKNOWN_TEXT[] SQ_CONST_VARIABLE = "Unknown";

const char * digital_mode_get ( int _pin )
{
   switch ( s_pins[_pin].m_type )
   {
   case DIGITAL_PIN_INPUT:
      return sq_get_constant_string(DIGITAL_PIN_INPUT_TEXT);

   case DIGITAL_PIN_OUTPUT:
      return sq_get_constant_string(DIGITAL_PIN_OUTPUT_TEXT);
      
   case DIGITAL_PIN_TIMER:
      return sq_get_constant_string(DIGITAL_PIN_TIMER_TEXT);
      
   default:
      return sq_get_constant_string(DIGITAL_PIN_UNKNOWN_TEXT);
   }
}

void digital_mode_set ( int _pin, const char * _mode )
{
   if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_INPUT_TEXT ) == 0 )
   {
      s_pins[_pin].m_type = DIGITAL_PIN_INPUT;
   }
   else if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_OUTPUT_TEXT ) == 0 )
   {
      s_pins[_pin].m_type = DIGITAL_PIN_OUTPUT;
   }
   else if ( SQ_CONSTANT_STRCMP ( _mode, DIGITAL_PIN_TIMER_TEXT ) == 0 )
   {
      s_pins[_pin].m_type = DIGITAL_PIN_TIMER;
   }
   else
   {
      sq_logf ( "Trying to set illegal mode: %s", _mode );
      s_pins[_pin].m_type = DIGITAL_PIN_OUTPUT;
      sq_digital_mode_updated ( _pin, digital_mode_get(_pin) );
   }
   
   switch ( s_pins[_pin].m_type )
   {
   case DIGITAL_PIN_INPUT:
      pinMode ( _pin, INPUT );
      s_pins[_pin].m_pin = digitalRead ( _pin ) == HIGH;
      sq_digital_pin_updated ( _pin, s_pins[_pin].m_pin );
      break;

   case DIGITAL_PIN_OUTPUT:
      pinMode ( _pin, OUTPUT );
      digitalWrite ( _pin, LOW );
      break;
      
   case DIGITAL_PIN_TIMER:
      pinMode ( _pin, INPUT );
      s_pins[_pin].m_counter = 0;
      s_pins[_pin].m_pin = digitalRead ( _pin ) == HIGH;
      sq_digital_counter_updated ( _pin, 0 );
      break;
   }
}

int digital_counter_get ( int _pin )
{
   return s_pins[_pin].m_counter;
}

void digital_counter_set ( int _pin, int _counter )
{
   s_pins[_pin].m_counter = _counter;
}

SQBool digital_pin_get ( int _pin )
{
   return s_pins[_pin].m_pin;
}

void digital_pin_set ( int _pin, SQBool _value )
{
   if ( s_pins[_pin].m_type == DIGITAL_PIN_OUTPUT )
   {
      digitalWrite ( _pin, _value == SQ_TRUE ? HIGH : LOW );
   }
   else
   {
      sq_logf ( "Trying to set pin %i, which is not an output pin.", _pin );
   }
}

int main ( void )
{
   SQBool newValue;
   int i;
   for ( i = 0; i < NUMBER_OF_PINS; i ++ )
   {
      pinMode ( i, OUTPUT );
      s_pins[i].m_type = DIGITAL_PIN_OUTPUT;
      s_pins[i].m_counter = 0;
   }
   
   static SQServer server;
   
   sq_init ();   
   
   sq_server_init ( &server, 4321 );
   
   while ( SQ_TRUE )
   {
      sq_server_poll ( &server );

      for ( i = 0; i < NUMBER_OF_PINS; i ++ )
      {
         switch ( s_pins[i].m_type )
         {
         case DIGITAL_PIN_TIMER:
            newValue = digitalRead ( i ) == HIGH;
            if ( s_pins[i].m_pin != newValue )
            {
               s_pins[i].m_pin = newValue;
               
               // If we go from high to low, the change should be counted
               if ( s_pins[i].m_pin == SQ_FALSE )
               {
                  s_pins[i].m_counter += 1;
               }
            }
            break;
            
         case DIGITAL_PIN_INPUT:
            newValue = digitalRead ( i ) == HIGH;
            if ( s_pins[i].m_pin != newValue )
            {
               s_pins[i].m_pin = newValue;
               
               sq_digital_pin_updated ( i, newValue );
            }
            break;
            
         default:
            break;
         }
      }
   }
   
   sq_shutdown ();   
}
