#include "acme_ventilator.h"

const int VENTILATOR_PIN = 13;
static SQBool s_ventilating = SQ_TRUE;

static enum VentilatorMode
{
   VENTILATOR_MODE_OFF = 0,
   VENTILATOR_MODE_ALWAYS_ON = 1,
   VENTILATOR_MODE_AT_NIGHT = 2,
   VENTILATOR_MODE_15_MINUTES_EACH_HOUR= 3,
   VENTILATOR_MODE_ONLY_WEEKDAYS = 4,
} s_mode = VENTILATOR_MODE_ALWAYS_ON;

static void ventilator_ventilating_set ( SQBool _value );

void ventilator_setup ( void )
{
   pinMode(VENTILATOR_PIN, OUTPUT);
   digitalWrite ( VENTILATOR_PIN, HIGH );
   ventilator_ventilating_set ( SQ_TRUE );
}

void ventilator_second_tick ( void )
{
   int weekday = clock_get_weekday();
   int hour = clock_get_hour();
   int minute = clock_get_minute();
   
   switch ( s_mode )
   {
   case VENTILATOR_MODE_OFF:
      ventilator_ventilating_set ( SQ_FALSE );
      break;
      
   case VENTILATOR_MODE_ALWAYS_ON:
      ventilator_ventilating_set ( SQ_TRUE );
      break;
      
   case VENTILATOR_MODE_AT_NIGHT:
      if ( hour < 7 && hour > 23 )
      {
         ventilator_ventilating_set ( SQ_TRUE );
      }
      else
      {
         ventilator_ventilating_set ( SQ_FALSE );
      }
      break;
      
   case VENTILATOR_MODE_15_MINUTES_EACH_HOUR:
      if ( minute < 15 )
      {
         ventilator_ventilating_set ( SQ_TRUE );
      }
      else
      {
         ventilator_ventilating_set ( SQ_FALSE );
      }
      break;
      
   case VENTILATOR_MODE_ONLY_WEEKDAYS:
      if ( weekday > 2 )
      {
         ventilator_ventilating_set ( SQ_TRUE );
      }
      else
      {
         ventilator_ventilating_set ( SQ_FALSE );
      }
      break;
   }
}

void ventilator_mode_set ( int _mode )
{
   if ( s_mode != _mode )
   {
      s_mode = _mode;
      
      sq_ventilator_mode_updated ( s_mode );
   }
}

int ventilator_mode_get ( void )
{
   return s_mode;
}


static void ventilator_ventilating_set ( SQBool _value )
{
   if ( _value != s_ventilating )
   {
      s_ventilating = _value;
      
      if ( s_ventilating == SQ_TRUE )
      {
         digitalWrite ( VENTILATOR_PIN, HIGH );
      }
      else
      {
         digitalWrite ( VENTILATOR_PIN, LOW );
      }
      
      sq_ventilator_ventilating_updated ( s_ventilating );
   }
}

SQBool ventilator_ventilating_get ( void )
{
   return s_ventilating;
}
