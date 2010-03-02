#include "common.h"
#include "Wire.h"

#include "statemanager.h"
#include "clock.h"
#include "LCD03.h"

#include "states.h"
#include "tempcontrol.h"

char responsebuffer[40];

const int heaterPin = 13;

extern "C"
{

const char * svn_revision ( void )
{
   return sq_get_constant_string(SQ_STRING_CONSTANT(SVN_REVISION));
}

}

StateManager statemanager(Idle);

void KeyHandler()
{
  static char lastKey = 0;
  static unsigned long pressedTime = 0;
  static unsigned long lastTime = 0;
  static unsigned long repeatTime = 0;
  char chIn = GetKeyPressed();
  if (chIn != lastKey)
  {
    if (chIn != 0)
    {
      statemanager.HandleKeyPressed(chIn);
      repeatTime = 750;
      pressedTime = lastTime = millis();
    }
    else
    {
      statemanager.HandleKeyReleased(lastKey, millis() - pressedTime);
    }
    lastKey = chIn;
  }
  else
  {
    unsigned long now = millis();
    if ( lastKey != 0 && (now - lastTime) > repeatTime)
    {
        statemanager.HandleKeyRepeated(lastKey, now - pressedTime);
        lastTime += repeatTime;
        repeatTime = 150;
    }
  }
}

unsigned long lastTick = 0;
SQServer server;

int main(int _argc, char * _argv[] )
{
	pinMode(heaterPin, OUTPUT);
  lastTick = millis();
	Wire.begin();
  SetupClock();
  sq_init ();
   
  sq_server_init ( &server, 4321 );
   
  while ( SQ_TRUE )
  {
		UpdateClock();
		sq_server_poll ( &server );
		statemanager.OnIdle();
		if ( (millis() - lastTick) >= 1000)
		{
			statemanager.HandleSecondTick();
			lastTick += 1000;
			TemperatureRegulation();
		}
		//KeyHandler();
	}
   
   sq_shutdown ();   
}
