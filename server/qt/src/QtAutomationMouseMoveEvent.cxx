#include <sequanto/QtAutomationMouseMoveEvent.h>

using namespace sequanto::automation;

const int QtAutomationMouseMoveEvent::ID = QEvent::registerEventType();

QtAutomationMouseMoveEvent::QtAutomationMouseMoveEvent ( int _x, int _y )
   : QtAutomationDelayedDoneEvent( (QEvent::Type) ID),
     m_position(_x, _y)
{
}

QtAutomationMouseMoveEvent::QtAutomationMouseMoveEvent ( const QtAutomationMouseMoveEvent & _toClone )
   : QtAutomationDelayedDoneEvent ( _toClone ),
     m_position ( _toClone.m_position )
{
}

QtAutomationMouseMoveEvent * QtAutomationMouseMoveEvent::clone () const
{
   return new QtAutomationMouseMoveEvent ( *this );
}
 
const QPoint & QtAutomationMouseMoveEvent::position()
{
   return m_position;
}

QtAutomationMouseMoveEvent::~QtAutomationMouseMoveEvent()
{
}
