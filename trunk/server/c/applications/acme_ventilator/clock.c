#include "acme_ventilator.h"

static byte clockSecond = 0;
static byte clockMinute = 0;
static byte clockHour = 0;
static byte clockDayOfWeek = 0;  // Monday = 0.
static unsigned long lastMilli = 0;
static unsigned long millisOnSecond = 1000;	// How many milliseconds in one second; normally 1000.

void clock_setup( void ) 
{
   lastMilli = millis();
}

unsigned long clock_now( void )
{
	return clockHour * 10000L + clockMinute * 100L + clockSecond;
}

SQBool clock_update( void )
{
   SQBool updated = SQ_FALSE;
   while (millis()-lastMilli >= millisOnSecond)
   {
      updated = SQ_TRUE;
      lastMilli += millisOnSecond;
      clockSecond++;
      if (clockSecond >= 60)
      {
         clockSecond = 0;
         clockMinute++;
         if (clockMinute >= 60)
         {
            clockMinute = 0;
            clockHour++;
            if (clockHour >= 24)
            {
               clockHour = 0;
               clockDayOfWeek++;
               if (clockDayOfWeek >= 7)
               {
                  clockDayOfWeek = 0;
               }
					//sq_clock_weekday_updated( clockDayOfWeek );
            }
				//sq_clock_hour_updated( clockHour );
         }
			//sq_clock_minute_updated( clockMinute );
      }
		if ((clockSecond % 10) == 0)
      {
         //sq_clock_second_updated( clockSecond);
      }
   }
   return updated;
}

const char* clock_nowtext ( void )
{
   sprintf ( s_responseBuffer, "%02u:%02u:%02u", clockHour, clockMinute, clockSecond );
   return s_responseBuffer;
}

int clock_get_weekday ( void )
{
   return clockDayOfWeek;
}

void clock_set_weekday ( int _weekday )
{
   if (_weekday != clockDayOfWeek)
   {
      clockDayOfWeek = _weekday;
      //sq_clock_weekday_updated( clockDayOfWeek);
   }
}

int clock_get_hour ( void )
{
   return clockHour;
}

void clock_set_hour ( int _hour )
{
   if (_hour != clockHour)
   {
      clockHour = _hour;
      //sq_clock_hour_updated( clockHour);
   }
}

int clock_get_minute ( void )
{
   return clockMinute;
}

void clock_set_minute ( int _minute )
{
   if (_minute != clockMinute)
   {
      clockMinute = _minute;
      //sq_clock_minute_updated( clockMinute);
   }
}

int clock_get_second ( void )
{
   return clockSecond;
}

void clock_set_second ( int _second )
{
   lastMilli = millis();	// To set exact time of second. This also ensures the second is not changing within the next 1000ms.
   if (_second != clockSecond)
   {
      clockSecond = _second;
      //sq_clock_second_updated( clockSecond);
   }
}
