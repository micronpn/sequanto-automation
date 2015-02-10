#include "acme_ventilator.h"

const int OUTSIDE_PIN = 0;

static float s_setpoint = 10.0f;
static float s_setpoint_critical = 20.0f;

void temperature_setup ( void )
{
   pinMode ( OUTSIDE_PIN, INPUT );
   s_setpoint = 10.0f;
   s_setpoint_critical = 20.0f;
}

int temperature_outside_raw_get ( void )
{
   return analogRead(OUTSIDE_PIN);
}

float temperature_outside_get ( void )
{
   int raw = temperature_outside_raw_get ();
   float percentage = (((float) raw) / 1053.0f) * 100.0f - 50.0f;
   return percentage;
}

float temperature_setpoint_get ( void )
{
   return s_setpoint;
}

void temperature_setpoint_set ( float _value )
{
   s_setpoint = _value;
}

float temperature_setpoint_critical_get ( void )
{
   return s_setpoint_critical;
}

void temperature_setpoint_critical_set ( float _value )
{
   s_setpoint_critical = _value;
}
