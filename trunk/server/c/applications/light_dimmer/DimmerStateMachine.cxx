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
   : StateMachine<DimmerState, DimmerMessage> ( DIMMER_INCREASING_OFF )
{
   m_currentLevel = 0;
   m_savedLevel = 0;
   m_sendOutput = true;
   m_firstHold = true;
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
   
   sq_dimmer_level_updated ( m_currentLevel );
   updateDisplay ();
}
   
int DimmerStateMachine::savedLevel () const
{
   return m_savedLevel;
}

void DimmerStateMachine::setSavedLevel ( int _level )
{
   m_savedLevel = _level;
   
   sq_dimmer_saved_level_updated ( m_savedLevel );
}
   
void DimmerStateMachine::handle ( DimmerMessage _message )
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

   case DIMMER_RELEASE:
      m_firstHold = true;
      
      switch( state() )
      {
      case DIMMER_INCREASING_ON:
         setState ( DIMMER_DECREASING_ON );
         break;

      case DIMMER_DECREASING_ON:
         setState ( DIMMER_INCREASING_ON );
         break;

      default:
         sq_logf ( "DIMMER_RELASE ERROR: %i", state() );
         break;
      }
      break;
      
   case DIMMER_HOLD:
      switch ( state() )
      {
      case DIMMER_INCREASING_ON:
      if ( m_firstHold )
      {
         setCurrentLevel ( m_currentLevel );
         m_firstHold = false;
      }
      else
      {
         if ( m_currentLevel < MAX_LEVEL )
         {
            setCurrentLevel ( m_currentLevel + 1 );
         }
         else
         {
            setCurrentLevel ( MAX_LEVEL - 1 );
            setState ( DIMMER_DECREASING_ON );
         }
      }
         break;
         
      case DIMMER_DECREASING_ON:
      if ( m_firstHold )
      {
         setCurrentLevel ( m_currentLevel );
         m_firstHold = false;
      }
      else
      {
         if ( m_currentLevel > 0 )
         {
            setCurrentLevel ( m_currentLevel - 1 );
         }
         else
         {
            setCurrentLevel ( 1 );
            setState ( DIMMER_INCREASING_ON );
         }
      }
         break;

      case DIMMER_INCREASING_OFF:
         /*
         if ( m_currentLevel == MAX_LEVEL )
         {
            setCurrentLevel ( m_currentLevel - 1 );
            setState ( DIMMER_DECREASING_OFF );
         }
         else
         {
            setCurrentLevel ( m_currentLevel + 1 );
         }
         */
         setCurrentLevel ( savedLevel() );
         setState ( DIMMER_INCREASING_ON );
         break;
         
      case DIMMER_DECREASING_OFF:
         /*
         if ( m_currentLevel  == 1 )
         {
            setCurrentLevel ( 0 );
            setState ( DIMMER_INCREASING_OFF );
         }
         else
         {
            setCurrentLevel ( m_currentLevel - 1 );
         }
         */
         setCurrentLevel ( savedLevel() );
         setState ( DIMMER_DECREASING_ON );
         break;
      }
   }
}

DimmerStateMachine Dimmer::instance;
