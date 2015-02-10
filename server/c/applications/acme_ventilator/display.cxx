#include "../arduino_demo/Wire.h"
#include "acme_ventilator.h"
#include "LCD03.h"

static char s_buffer[10];

void display_setup ( void )
{
   Wire.begin();
}

void display_send ()
{
   display_update ( SQ_TRUE );
}

void display_set_line ( int _line, const char * _data, SQBool _sendUpdate )
{
   PutString ( _line, 1, _data );
   if ( _sendUpdate == SQ_TRUE )
   {
      switch ( _line )
      {
      case 0:
         sq_display_line1_updated ( _data );
         break;

      case 1:
         sq_display_line2_updated ( _data );
         break;

      case 2:
         sq_display_line3_updated ( _data );
         break;

      case 3:
         sq_display_line4_updated ( _data );
         break;

      }
   }
}

void display_update ( SQBool _sendUpdate )
{
   Cls();
   DisplayCommand(4);
   DisplayCommand(19);

   int minute = clock_get_minute();
   int hour = clock_get_hour();
   int weekday = clock_get_weekday();
   float outside = temperature_outside_get ();
   int alarms = alarms_get();

   snprintf ( s_buffer, 10, "%.2i:%.2i", hour, minute );
   display_set_line ( 0, s_buffer, _sendUpdate );

   switch ( weekday )
   {
   case 0:
      display_set_line ( 1, "Monday", _sendUpdate );
      break;

   case 1:
      display_set_line ( 1, "Tuesday", _sendUpdate );
      break;

   case 2:
      display_set_line ( 1, "Wednesday", _sendUpdate );
      break;

   case 3:
      display_set_line ( 1, "Thursday", _sendUpdate );
      break;

   case 4:
      display_set_line ( 1, "Friday", _sendUpdate );
      break;

   case 5:
      display_set_line ( 1, "Saturday", _sendUpdate );
      break;

   case 6:
      display_set_line ( 1, "Sunday", _sendUpdate );
      break;

   default:
      display_set_line ( 1, "ERROR", _sendUpdate );
      break;
   }

   dtostrf ( outside, 5, 2, s_buffer );
   display_set_line ( 2, s_buffer, _sendUpdate );

   if ( alarms != 0 )
   {
      display_set_line ( 3, "ALARM", _sendUpdate );
   }
}
