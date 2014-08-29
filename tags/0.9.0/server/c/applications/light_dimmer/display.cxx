#include "config.h"

#include <Wire.h>
#include <LCD03.h>

#include "InterfaceStateMachine.h"
#include "DimmerStateMachine.h"

void initDisplay ()
{
   Cls ();
   PutString ( 1, 1, "Sequanto LightDimmer" );
   PutString ( 2, 1, " v. " SQ_SVN_REVISION );
   PutString ( 3, 1, "Level: " );
}

void updateDisplay ()
{
   SetCursorPosition ( 3, 8 );
   if ( Dimmer::instance.currentLevel() == 10 )
   {
      PutChar ( '1' );
      PutChar ( '0' );
   }
   else
   {
      PutChar ( ' ' );
      PutChar ( '0' + Dimmer::instance.currentLevel() );
   }
   SetCursorPosition ( 4, 20 );
   switch ( Interface::instance.state() )
   {
   case INTERFACE_GRASPED:
      PutChar ( 'G' );
      break;

   case INTERFACE_HOLDING:
      PutChar ( 'H' );
      break;
      
   case INTERFACE_NOT_GRASPED:
      PutChar ( ' ' );
      break;

   case INTERFACE_IGNORING:
      PutChar ( 'I' );
      break;
   }
}

