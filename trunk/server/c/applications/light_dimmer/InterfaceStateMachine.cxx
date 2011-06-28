#include <sequanto/automation.h>

#include "InterfaceStateMachine.h"
#include "light_dimmer_automation.h"
#include "display.h"
#include "DimmerStateMachine.h"

void InterfaceStateMachine::setState ( InterfaceState _state )
{
   StateMachine<InterfaceState, InterfaceMessage>::setState ( _state );
      
   sq_statemachines_interface_state_updated ( _state );
   updateDisplay();
}

InterfaceStateMachine::InterfaceStateMachine ()
   : StateMachine<InterfaceState, InterfaceMessage> ( INTERFACE_NOT_GRASPED )
{
   m_ticksSinceGrasp = 0;
   m_ticksSinceHold = 0;
}
   
void InterfaceStateMachine::handle ( InterfaceMessage _message )
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
            Dimmer::instance.handle ( DIMMER_HOLD );
            m_ticksSinceHold = 0;
         }
         break;
            
      case INTERFACE_RELEASE:
         if ( m_ticksSinceGrasp > ON_OFF_DELAY && m_ticksSinceGrasp < LEVEL_DELAY )
         {
            Dimmer::instance.handle( DIMMER_TOUCH );
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

InterfaceStateMachine Interface::instance;
