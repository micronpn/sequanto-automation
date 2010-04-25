#include "common.h"
#include "clock.h"
extern "C"
{
#include "arduino_demo_automation.h"
}

static byte clockSecond = 0;
static byte clockMinute = 0;
static byte clockHour = 0;
static byte clockDayOfWeek = 0;  // Monday = 0.
static unsigned long lastMilli = 0;
static unsigned long millisOnSecond = 1000;	// How many milliseconds in one second; normally 1000.

static ALARM* alarms[8] = {null, null, null, null, null, null, null, null };

void SetupClock() 
{
  lastMilli = millis();
}

unsigned long Now()
{
	return clockHour * 10000L + clockMinute * 100L + clockSecond;
}

boolean CheckAlarm( struct ALARM *pAlarm )
{
  if (
    pAlarm != null &&
    pAlarm->enabled == true &&
    pAlarm->hour == clockHour &&
    pAlarm->minute == clockMinute &&
    pAlarm->second == clockSecond)
  {
    pAlarm->set = true;
    if (pAlarm->pAlarm != null) pAlarm->pAlarm(pAlarm);
    return true;
  }
  return false;
}

boolean UpdateClock()
{
  boolean b = false;
  while (millis()-lastMilli >= millisOnSecond)
  {
    b = true;
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
					sq_clock_weekday_updated( clockDayOfWeek );
        }
				sq_clock_hour_updated( clockHour );
      }
			sq_clock_minute_updated( clockMinute );
    }
		if ((clockSecond % 10) == 0) sq_clock_second_updated( clockSecond);
		CheckAlarm(alarms[0]);
    CheckAlarm(alarms[1]);
    CheckAlarm(alarms[2]);
    CheckAlarm(alarms[3]);
    CheckAlarm(alarms[4]);
    CheckAlarm(alarms[5]);
    CheckAlarm(alarms[6]);
    CheckAlarm(alarms[7]);
  }
  return b;
}

int GetClockString( char *buffer )
{
  return sprintf(buffer, "%02u:%02u:%02u", clockHour, clockMinute, clockSecond);
}


//1 Automation

extern "C"
{

int clock_now( void )
{
	return (int)(Now() / 100L);
}

char* clock_nowtext ( void )
{
	GetClockString(responsebuffer);
	return responsebuffer;
}

int get_weekday ( void )
{
   return clockDayOfWeek;
}

void set_weekday ( int _weekday )
{
	if (_weekday != clockDayOfWeek)
	{
		clockDayOfWeek = _weekday;
		sq_clock_weekday_updated( clockDayOfWeek);
	}
}

int get_hour ( void )
{
   return clockHour;
}

void set_hour ( int _hour )
{
	if (_hour != clockHour)
	{
		clockHour = _hour;
		sq_clock_hour_updated( clockHour);
	}
}

int get_minute ( void )
{
   return clockMinute;
}

void set_minute ( int _minute )
{
	if (_minute != clockMinute)
	{
		clockMinute = _minute;
		sq_clock_minute_updated( clockMinute);
	}
}

int get_second ( void )
{
   return clockSecond;
}

void set_second ( int _second )
{
  lastMilli = millis();	// To set exact time of second. This also ensures the second is not changing within the next 1000ms.
	if (_second != clockSecond)
	{
		clockSecond = _second;
		sq_clock_second_updated( clockSecond);
	}
}

}
