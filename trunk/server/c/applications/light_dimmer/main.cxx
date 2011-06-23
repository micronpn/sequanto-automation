#include <sequanto/automation.h>
#include <avr/io.h>
#include <LCD03.h>
#include <Wire.h>

#include "light_dimmer_automation.h"
#include "StateMachine.h"

#include "wiring.h"

static const int ON_OFF_DELAY = 50;
static const int LEVEL_DELAY = 1000;
static const int MAX_LEVEL = 10;

void updateDisplay ();

typedef enum _DimmerState
{
   DIMMER_INCREASING_ON = 1,
   DIMMER_INCREASING_OFF = 2,
   DIMMER_DECREASING_ON = 4,
   DIMMER_DECREASING_OFF = 8,
} DimmerState;

typedef enum _DimmerMessage
{
   DIMMER_HOLD,
   DIMMER_TOUCH,
   DIMMER_RESET,
} DimmerMessage;

class DimmerStateMachine : public StateMachine<DimmerState, DimmerMessage>
{
private:
   int m_currentLevel;
   int m_savedLevel;

   void setState ( DimmerState _state )
   {
      StateMachine<DimmerState, DimmerMessage>::setState ( _state );
      
      sq_statemachines_dimmer_state_updated ( _state );
   }
   
public:
   DimmerStateMachine ()
      : StateMachine<DimmerState, DimmerMessage> ( DIMMER_INCREASING_OFF )
   {
      m_currentLevel = 0;
      m_savedLevel = 0;
   }
   
   int currentLevel () const
   {
      return m_currentLevel;
   }
   
   void setCurrentLevel ( int _level )
   {
      m_currentLevel = _level;
      
      sq_dimmer_level_updated ( m_currentLevel );
      updateDisplay ();
   }
   
   int savedLevel () const
   {
      return m_savedLevel;
   }

   void setSavedLevel ( int _level )
   {
      m_savedLevel = _level;
      
      sq_dimmer_saved_level_updated ( m_savedLevel );
   }
   
   void handle ( DimmerMessage _message )
   {
      switch ( _message )
      {
      case DIMMER_TOUCH:
         switch ( state() )
         {
         case DIMMER_INCREASING_ON:
            setSavedLevel ( m_currentLevel );
            setCurrentLevel ( 0 );
            setState ( DIMMER_INCREASING_OFF );
            break;

         case DIMMER_DECREASING_ON:
            setSavedLevel ( m_currentLevel );
            setCurrentLevel ( 0 );
            setState ( DIMMER_DECREASING_OFF );
            break;
            
         case DIMMER_INCREASING_OFF:
            setCurrentLevel ( m_savedLevel );
            setState ( DIMMER_INCREASING_ON );
            break;

         case DIMMER_DECREASING_OFF:
            setCurrentLevel ( m_savedLevel );
            setState ( DIMMER_DECREASING_ON );
            break;
         }
         //sq_logf ( "Touch, saved level = %i", m_savedLevel );
         break;
         
      case DIMMER_RESET:
         setCurrentLevel ( 0 );
         setSavedLevel ( 0 );
         setState ( DIMMER_INCREASING_OFF );
         break;
         
      case DIMMER_HOLD:
         switch ( state() )
         {
         case DIMMER_INCREASING_ON:
         case DIMMER_INCREASING_OFF:
            setCurrentLevel ( m_currentLevel + 1 );
            if ( m_currentLevel == MAX_LEVEL )
            {
               setState ( DIMMER_DECREASING_ON );
            }
            else if ( state() == DIMMER_INCREASING_OFF )
            {
               setState ( DIMMER_DECREASING_ON );
            }
            break;
            
         case DIMMER_DECREASING_ON:
         case DIMMER_DECREASING_OFF:
            if ( m_currentLevel - 1 == 0 )
            {
               setCurrentLevel ( 0 );
               setState ( DIMMER_INCREASING_ON );
            }
            else
            {
               setCurrentLevel ( m_currentLevel - 1 );
               if ( state() == DIMMER_DECREASING_OFF )
               {
                  setState ( DIMMER_INCREASING_ON );
               }
            }
            break;
         }
      }
   }
};

DimmerStateMachine dimmer;

typedef enum _InterfaceState
{
   INTERFACE_NOT_GRASPED = 0,
   INTERFACE_GRASPED = 1,
} InterfaceState;

typedef enum _InterfaceMessage
{
   INTERFACE_GRASP,
   INTERFACE_RELEASE,
   INTERFACE_TICK,
   INTERFACE_RESET,
} InterfaceMessage;

class InterfaceStateMachine : public StateMachine<InterfaceState, InterfaceMessage>
{
private:
   int m_ticksSinceGrasp;
   int m_ticksSinceHold;
   
   void setState ( InterfaceState _state )
   {
      StateMachine<InterfaceState, InterfaceMessage>::setState ( _state );
      
      sq_statemachines_interface_state_updated ( _state );
      updateDisplay();
   }
   
public:
   InterfaceStateMachine ()
      : StateMachine<InterfaceState, InterfaceMessage> ( INTERFACE_NOT_GRASPED )
   {
      m_ticksSinceGrasp = 0;
      m_ticksSinceHold = 0;
   }
   
   void handle ( InterfaceMessage _message )
   {
      switch ( state() )
      {
      case INTERFACE_GRASPED:
         switch ( _message )
         {
         case INTERFACE_TICK:
            m_ticksSinceGrasp ++;
            m_ticksSinceHold ++;
            if ( m_ticksSinceHold > LEVEL_DELAY )
            {
               dimmer.handle ( DIMMER_HOLD );
               m_ticksSinceHold = 0;
            }
            break;
            
         case INTERFACE_RELEASE:
            if ( m_ticksSinceGrasp > ON_OFF_DELAY && m_ticksSinceGrasp < LEVEL_DELAY )
            {
               dimmer.handle( DIMMER_TOUCH );
            }
            m_ticksSinceGrasp = 0;
            m_ticksSinceHold = 0;
            setState ( INTERFACE_NOT_GRASPED );
            sq_interface_grasped_updated ( SQ_FALSE );
            break;

         default:
            break;
         }
         break;
         
      case INTERFACE_NOT_GRASPED:
         switch ( _message )
         {
         case INTERFACE_GRASP:
            m_ticksSinceGrasp = 0;
            m_ticksSinceHold = 0;
            setState ( INTERFACE_GRASPED );
            sq_interface_grasped_updated ( SQ_TRUE );
            break;
            
         default:
            break;
         }
         break;
      }
      
      if ( _message == INTERFACE_RESET )
      {
         m_ticksSinceGrasp = 0;
         setState ( INTERFACE_NOT_GRASPED );
      }
   }
};

InterfaceStateMachine interface;

void initDisplay ()
{
   Cls ();
   PutString ( 1, 1, "Sequanto LightDimmer" );
   PutString ( 2, 1, " v. " SVN_REVISION );
   PutString ( 3, 1, "Level: " );
}

void updateDisplay ()
{
   SetCursorPosition ( 3, 8 );
   if ( dimmer.currentLevel() == 10 )
   {
      PutChar ( '1' );
      PutChar ( '0' );
   }
   else
   {
      PutChar ( ' ' );
      PutChar ( '0' + dimmer.currentLevel() );
   }
   SetCursorPosition ( 4, 20 );
   if ( interface.state() == INTERFACE_GRASPED )
   {
      PutChar ( 'G' );
   }
   else
   {
      PutChar ( ' ' );
   }
}

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
      return dimmer.currentLevel ();
   }
   
   void dimmer_set_current_level ( int _level )
   {
      dimmer.setCurrentLevel ( _level );
   }
   
   int dimmer_get_saved_level ( void )
   {
      return dimmer.savedLevel ();
   }

   void dimmer_set_saved_level ( int _level )
   {
      dimmer.setSavedLevel ( _level );
   }
   
   SQBool interface_get_grasped ( void )
   {
      return interface.state() == INTERFACE_GRASPED;
   }
   
   void interface_grasp ( void )
   {
      interface.handle ( INTERFACE_GRASP );
   }
   
   void interface_release ( void )
   {
      interface.handle ( INTERFACE_RELEASE );
   }
   
   void reset (void )
   {
      interface.handle ( INTERFACE_RESET );
      dimmer.handle ( DIMMER_RESET );
   }
   
   void hard_reset (void )
   {
      g_keepRunning = SQ_FALSE;
   }
   
   int get_interface_state ( void )
   {
      return interface.state();
   }

   int get_dimmer_state ( void )
   {
      return dimmer.state();
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
   
   //pinMode ( i, INPUT);
   //digitalWrite ( i, HIGH );

   sq_init ();   
   
   sq_server_init ( &server, 4321 );
   
   Wire.begin();
   
   initDisplay();
   
   reset ();
   
   lastNow = millis();
   lastKey = 0;
   
   while ( g_keepRunning == SQ_TRUE )
   {
      sq_server_poll ( &server );
      
      now = millis();
      if ( lastNow != now )
      {
         diff = now - lastNow;
         while ( diff > 0 )
         {
            interface.handle ( INTERFACE_TICK );
            //dimmer.handle ( DIMMER_TICK );
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
            interface.handle ( INTERFACE_GRASP );
            break;
            
         default:
            interface.handle ( INTERFACE_RELEASE );
            break;
         }
         
         lastKey = key;
      }
   }
   
   sq_shutdown ();
}
