#include "acme_ventilator.h"

char s_responseBuffer[40];

const char * main_svn_revision ( void )
{
   return sq_get_constant_string(SQ_STRING_CONSTANT(SQ_SVN_REVISION));
}

static const char PRODUCT[] SQ_CONST_VARIABLE = "Acme V200-B1";
const char * main_product ( void )
{
   return sq_get_constant_string(PRODUCT);
}

static const char SERIAL_NUMBER[] SQ_CONST_VARIABLE = "R2201334";
const char * main_serial_number ( void )
{
   return sq_get_constant_string(SERIAL_NUMBER);
}

int s_alarms;
int alarms_get ( void )
{
   return s_alarms;
}
void alarms_set ( int _alarms )
{
   s_alarms = _alarms;
}

int main(int _argc, char * _argv[] )
{
   unsigned long lastTick = 0;
   SQServer server;
   
   lastTick = millis();
   
   display_setup();
   ventilator_setup();
   clock_setup();
   temperature_setup ();
   
   sq_init ();
   sq_server_init ( &server, 4321 );
   
   while ( SQ_TRUE )
   {
		clock_update();
		sq_server_poll ( &server );
		if ( (millis() - lastTick) >= 1000)
		{
			ventilator_second_tick();
         display_update();
			lastTick += 1000;
		}
	}
   
   sq_shutdown ();   
}
