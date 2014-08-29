#include "../arduino_demo/Wire.h"
#include "acme_ventilator.h"
#include "LCD03.h"

static char s_buffer[10];

void display_setup ( void )
{
   Wire.begin();
}

SQBool send_display = SQ_FALSE;

void display_send ()
{
   send_display = SQ_TRUE;
   display_update ();
   send_display = SQ_FALSE;
}

void display_set_line ( int _line, const char * _data )
{
   PutString ( _line, 1, _data );
   if ( send_display == SQ_TRUE )
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

void display_update ( void )
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
   display_set_line ( 0, s_buffer );
   
   switch ( weekday )
   {
   case 0:
      display_set_line ( 1, "Monday" );
      break;

   case 1:
      display_set_line ( 1, "Tuesday" );
      break;

   case 2:
      display_set_line ( 1, "Wednesday" );
      break;

   case 3:
      display_set_line ( 1, "Thursday" );
      break;

   case 4:
      display_set_line ( 1, "Friday" );
      break;

   case 5:
      display_set_line ( 1, "Saturday" );
      break;

   case 6:
      display_set_line ( 1, "Sunday" );
      break;

   default:
      display_set_line ( 1, "ERROR" );
      break;
   }
   
   dtostrf ( outside, 5, 2, s_buffer );
   display_set_line ( 2, s_buffer );
   
   if ( alarms != 0 )
   {
      display_set_line ( 3, "ALARM" );
   }
}
