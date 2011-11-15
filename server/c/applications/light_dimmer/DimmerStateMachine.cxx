#include <sequanto/automation.h>

#include "DimmerStateMachine.h"
#include "display.h"
#include "light_dimmer_automation.h"

void DimmerStateMachine::setState ( DimmerState _state )
{
   StateMachine<DimmerState, DimmerMessage>::setState ( _state );
   
   sq_statemachines_dimmer_state_updated ( _state );
}
   
DimmerStateMachine::DimmerStateMachine ()
   : StateMachine<DimmerState, DimmerMessage> ( DIMMER_UP_PASSIVE )
{
   m_currentLevel = 0;
   m_savedLevel = 0;
   m_sendOutput = true;
}
   
bool DimmerStateMachine::sendOutput ()
{
   return m_sendOutput;
}

int DimmerStateMachine::currentLevel () const
{
   return m_currentLevel;
}

int DimmerStateMachine::getCurrentLevelAutomation ()
{
   m_sendOutput = false;
   
   return m_currentLevel;
}
   
void DimmerStateMachine::setCurrentLevel ( int _level )
{
   m_currentLevel = _level;
   m_sendOutput = true;
   
   //sq_dimmer_level_updated ( m_currentLevel );
   updateDisplay ();
}
   
int DimmerStateMachine::savedLevel () const
{
   return m_savedLevel;
}

void DimmerStateMachine::setSavedLevel ( int _level )
{
   m_savedLevel = _level;
   
   //sq_dimmer_saved_level_updated ( m_savedLevel );
}
   
void DimmerStateMachine::handle ( DimmerMessage _message )
{
   if ( _message == DIMMER_TOUCH )
   {
      if ( m_lightOn )
      {
         setSavedLevel ( m_currentLevel );
         setCurrentLevel ( 0 );
         m_lightOn = false;
      }
      else
      {
         setCurrentLevel ( m_savedLevel );
         m_lightOn = true;
      }
   }
   else if ( _message == DIMMER_RESET )
   {
      m_currentLevel = 0;
      m_savedLevel = 0;
      m_lightOn = false;
      m_sendOutput = false;
   }
   else
   {
      switch ( state() )
      {
      case DIMMER_UP:
         switch ( _message )
         {
         case DIMMER_HOLD:
            if ( m_currentLevel < MAX_LEVEL )
            {
               setCurrentLevel ( m_currentLevel + 1 );
            }
            else
            {
               setCurrentLevel ( MAX_LEVEL - 1 );
               setState ( DIMMER_DOWN );
            }
            break;
         
         case DIMMER_STOP_HOLD:
            setSavedLevel ( m_currentLevel );
            setState ( DIMMER_DOWN_PASSIVE );
            break;

         default:
            sq_logf ( "DIMMER_UP, invalid message: %i", _message );
         }
         break;
      
      case DIMMER_UP_PASSIVE:
         switch ( _message )
         {
         case DIMMER_START_HOLD:
            setCurrentLevel ( m_savedLevel );
            setState ( DIMMER_UP );
            m_lightOn = true;
            break;

         default:
            sq_logf ( "DIMMER_UP_PASSIVE, invalid message: %i", _message );
         }
         break;
      
      case DIMMER_DOWN:
         switch ( _message )
         {
         case DIMMER_HOLD:
            if ( m_currentLevel > 0 )
            {
               setCurrentLevel ( m_currentLevel - 1 );
            }
            else
            {
               setCurrentLevel ( 1 );
               setState ( DIMMER_UP );
            }
            break;
         
         case DIMMER_STOP_HOLD:
            setSavedLevel ( m_currentLevel );
            setState ( DIMMER_UP_PASSIVE );
            break;

         default:
            sq_logf ( "DIMMER_DOWN, invalid message: %i", _message );
         }
         break;
            
      case DIMMER_DOWN_PASSIVE:
         switch ( _message )
         {
         case DIMMER_START_HOLD:
            setSavedLevel ( m_currentLevel );
            //setCurrentLevel ( m_currentLevel );
            setState ( DIMMER_DOWN );
            m_lightOn = true;
            break;

         default:
            sq_logf ( "DIMMER_DOWN_PASSIVE, invalid message: %i", _message );
         }
         break;
      }
   }
}

DimmerStateMachine Dimmer::instance;
