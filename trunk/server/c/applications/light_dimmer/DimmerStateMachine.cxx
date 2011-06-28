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
}
   
int DimmerStateMachine::currentLevel () const
{
   return m_currentLevel;
}
   
void DimmerStateMachine::setCurrentLevel ( int _level )
{
   m_currentLevel = _level;
      
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

DimmerStateMachine Dimmer::instance;
