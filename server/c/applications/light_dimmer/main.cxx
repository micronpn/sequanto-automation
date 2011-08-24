#include <sequanto/automation.h>
#include <avr/io.h>
#include <Wire.h>
#include <LCD03.h>

#include "light_dimmer_automation.h"
#include "InterfaceStateMachine.h"
#include "DimmerStateMachine.h"
#include "display.h"

#include "wiring.h"

SQBool g_keepRunning;

extern "C"
{
   const char * sequanto_automation_version ( void )
   {
      static char buff[20] = SVN_REVISION;
      /*   sq_get_constant_string_copy ( SQ_STRING_CONSTANT(SVN_REVISION), buff );*/
      return buff;
   }
   
   int dimmer_get_current_level ( void )
   {
      return Dimmer::instance.currentLevel ();
   }
   
   void dimmer_set_current_level ( int _level )
   {
      Dimmer::instance.setCurrentLevel ( _level );
   }
   
   int dimmer_get_saved_level ( void )
   {
      return Dimmer::instance.savedLevel ();
   }

   void dimmer_set_saved_level ( int _level )
   {
      Dimmer::instance.setSavedLevel ( _level );
   }
   
   SQBool interface_get_grasped ( void )
   {
      return Interface::instance.state() == INTERFACE_GRASPED;
   }
   
   void interface_grasp ( void )
   {
      Interface::instance.handle ( INTERFACE_GRASP );
   }
   
   void interface_release ( void )
   {
      Interface::instance.handle ( INTERFACE_RELEASE );
   }
   
   void reset (void )
   {
      Interface::instance.handle ( INTERFACE_RESET );
      Dimmer::instance.handle ( DIMMER_RESET );
   }
   
   void hard_reset (void )
   {
      g_keepRunning = SQ_FALSE;
   }
   
   int get_interface_state ( void )
   {
      return Interface::instance.state();
   }

   int get_dimmer_state ( void )
   {
      return Dimmer::instance.state();
   }
}

int main ( void )
{
   static SQServer server;
   unsigned long now;
   unsigned long lastNow;
   unsigned long diff;
   char key;
   char lastKey;
   
   g_keepRunning = SQ_TRUE;
   
   pinMode ( 13, OUTPUT);
   digitalWrite ( 13, HIGH );

   sq_init ();   
   
   sq_server_init ( &server, 4321 );

   digitalWrite ( 13, LOW );
   
   Wire.begin();
   
   digitalWrite ( 13, HIGH );
   
   initDisplay();

   digitalWrite ( 13, LOW );
   
   reset ();
   
      
   digitalWrite ( 13, HIGH );
   
   lastNow = millis();
   lastKey = 0;
   
   SQBool led = SQ_FALSE;
   while ( g_keepRunning == SQ_TRUE )
   {
      sq_server_poll ( &server );
      
      now = millis();
      
      if ( (now % 50) == 0 )
      {
         if ( led == SQ_TRUE )
         {
            led = SQ_FALSE;
         }
         else
         {
            led = SQ_TRUE;
         }
         digitalWrite ( 13, led == SQ_TRUE ? HIGH : LOW );
      }
      
      if ( lastNow != now )
      {
         diff = now - lastNow;
         while ( diff > 0 )
         {
            Interface::instance.handle ( INTERFACE_TICK );
            //Dimmer::instance.handle ( DIMMER_TICK );
            diff--;
         }
         lastNow = now;
      }
      
      key = GetKeyPressed ();
      if ( key != lastKey )
      {
         switch ( key )
         {
         case '*':
            Interface::instance.handle ( INTERFACE_GRASP );
            break;
            
         default:
            Interface::instance.handle ( INTERFACE_RELEASE );
            break;
         }
         
         lastKey = key;
      }
   }
   
   sq_shutdown ();
}
