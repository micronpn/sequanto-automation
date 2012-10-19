#include <sequanto/automation.h>
#include "acme_ventilator_automation.h"
#include "wiring.h"
#include <stdio.h>

void ventilator_setup ( void );
void ventilator_second_tick ( void );

void clock_setup( void );
SQBool clock_update( void );
int clock_get_weekday ( void );
int clock_get_hour ( void );
int clock_get_minute ( void );
int clock_get_second ( void );

extern char s_responseBuffer[40];
