#include <sequanto/automation.h>
#include "acme_ventilator_automation.h"
#include "wiring.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

void ventilator_setup ( void );
void ventilator_second_tick ( void );

void display_setup ( void );
void display_update ( SQBool _sendUpdate );

void clock_setup( void );
SQBool clock_update( void );
int clock_get_weekday ( void );
int clock_get_hour ( void );
int clock_get_minute ( void );
int clock_get_second ( void );

void temperature_setup ( void );
float temperature_outside_get ( void );
float temperature_setpoint_get ( void );
float temperature_setpoint_critical_get ( void );

int alarms_get ( void );
void alarms_set( int _alarms );

void display_send ( void );

extern char s_responseBuffer[40];

#ifdef __cplusplus
}
#endif
