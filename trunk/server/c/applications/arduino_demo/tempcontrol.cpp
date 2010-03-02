#include "common.h"
#include "clock.h"
#include "tempcontrol.h"
extern "C"
{
#include "arduino_demo_automation.h"
}

static float current_temperature = 18.4;
static int current_temperatureInt10 = 184;

static unsigned long day_clock = 70000;	// 7:00
static float day_temp = 21.0;

static SQBool night_used = SQ_TRUE;
static unsigned long night_clock = 3000;	// 0:30
static float night_temp = 16.0;


int ToInt10(float _value)
{
	return (int)round(_value * 10.0);
}

float FromInt10(int _value)
{
	return (float)_value * 0.1;
}

float ToTemperature( float _voltage )
{
	return (_voltage * 10.0) - 10.0;
}

int ReadTemperatureAdc( int pin )
{
	return analogRead(pin);
}

float ReadTemperatureVoltage( int pin )
{
	return (float)ReadTemperatureAdc(pin) * (5.0 / 1024.0);
}

float ReadCurrentTemperature()
{
	current_temperature = ToTemperature( ReadTemperatureVoltage(0) );
	int temp10 = ToInt10( current_temperature );
	if (temp10 != current_temperatureInt10)
	{
		current_temperatureInt10 = temp10;
		sq_temp_current_updated( server.m_stream, current_temperatureInt10 );
	}
	return current_temperature;
}

SQBool GetHeaterState(void)
{
	return (digitalRead(heaterPin) == HIGH) ? SQ_TRUE : SQ_FALSE;
}

void SetHeaterState( SQBool _state )
{
	if ( _state != GetHeaterState() )
	{
		digitalWrite( heaterPin, (_state == SQ_TRUE) ? HIGH : LOW );
		sq_temp_heater_updated( server.m_stream, _state );
	}
}

SQBool IsInDayTime( unsigned long clock )
{
	if (day_clock > night_clock)
	{
		if (clock >= night_clock && clock < day_clock) return SQ_FALSE;
		else return SQ_TRUE;
	}
	else
	{
		if (clock >= day_clock && clock < night_clock) return SQ_TRUE;
		else return SQ_FALSE;
	}
}

SQBool UsingDayTime( unsigned long clock )
{
	return ( IsInDayTime(Now()) || night_used == SQ_FALSE ) ? SQ_TRUE : SQ_FALSE;
}

int GetCurrentTemperatureSetpoint( void )
{
	if ( UsingDayTime(Now()) )
	{
		return day_temp;
	}
	else
	{
		return night_temp;
	}
}


void SetCurrentTemperatureSetpoint( float _temp )
{
	int inttemp = ToInt10(_temp);
	if ( UsingDayTime(Now()) )
	{
		day_temp = _temp;
		sq_temp_day_temp_updated( server.m_stream, inttemp );
	}
	else
	{
		night_temp = _temp;
		sq_temp_night_temp_updated( server.m_stream, inttemp );
	}
	sq_temp_setpoint_updated( server.m_stream, inttemp );
}


void TemperatureRegulation(void)
{
	float setpoint = GetCurrentTemperatureSetpoint();
	float temperature = ReadCurrentTemperature();
	if ( GetHeaterState() == SQ_TRUE )
	{
		if ( temperature > (setpoint + 0.3) )
		{
			SetHeaterState( SQ_FALSE );
		}
	}
	else
	{
		if ( temperature < (setpoint - 0.3) )
		{
			SetHeaterState( SQ_TRUE);
		}
	}
}


//1 Automation

extern "C"
{

int get_currenttemp ( void )
{
	return current_temperatureInt10;
}

void set_currenttemp( int _temp )
{
}

int read_temperature(void)
{
	return ToInt10( ReadCurrentTemperature() );
}

int read_voltage(void)
{
	return ToInt10( ReadTemperatureVoltage(0) );
}

SQBool get_heaterstate ( void )
{
	return GetHeaterState();
}

void set_heaterstate ( SQBool _state )
{
	SetHeaterState( _state );
}

int get_tempsetpoint ( void )
{
	return ToInt10( GetCurrentTemperatureSetpoint() );
}

void set_tempsetpoint ( int _temp )
{
	SetCurrentTemperatureSetpoint( FromInt10(_temp) );
}

int get_daytemp ( void )
{
	return ToInt10(day_temp);
}

void set_daytemp ( int _temp )
{
	day_temp = FromInt10(_temp);
	sq_temp_day_temp_updated( server.m_stream, _temp );
	if ( UsingDayTime(Now()) == SQ_TRUE)
	{
		sq_temp_setpoint_updated( server.m_stream, _temp );
	}
}

int get_dayclock ( void )
{
	return (int)(day_clock / 100L);
}

void set_dayclock ( int _clock )
{
	day_clock = 100L * (unsigned long)_clock;
	sq_temp_day_clock_updated( server.m_stream, _clock );
}

SQBool get_nighttempused ( void )
{
	return night_used;
}

void set_nighttempused ( SQBool _used )
{
	if (_used != night_used)
	{
		night_used = _used;
		sq_temp_night_used_updated( server.m_stream, _used );
	}
}

int get_nighttemp ( void )
{
	return ToInt10(night_temp);
}

void set_nighttemp ( int _temp )
{
	night_temp = FromInt10(_temp);
	sq_temp_night_temp_updated( server.m_stream, _temp );
	if ( UsingDayTime(Now()) == SQ_FALSE)
	{
		sq_temp_setpoint_updated( server.m_stream, _temp );
	}
}

int get_nightclock ( void )
{
	return (int)(night_clock / 100L);
}

void set_nightclock ( int _clock )
{
	night_clock = 100L * (unsigned long)_clock;
	sq_temp_night_clock_updated( server.m_stream, _clock );
}


}

