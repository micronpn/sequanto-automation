#include <sequanto/QtAutomationMouseClickEvent.h>

using namespace sequanto::automation;

const int QtAutomationMouseClickEvent::ID = QEvent::registerEventType();

QtAutomationMouseClickEvent::QtAutomationMouseClickEvent ( int _x, int _y, Qt::MouseButton _button )
   :  QtAutomationDelayedDoneEvent( (QEvent::Type) ID),
      m_position(_x, _y),
      m_button(_button)
{
}

QtAutomationMouseClickEvent::QtAutomationMouseClickEvent ( const QtAutomationMouseClickEvent & _toClone )
   : QtAutomationDelayedDoneEvent ( _toClone ),
     m_position ( _toClone.m_position ),
     m_button ( _toClone.m_button )
{
}

QtAutomationMouseClickEvent * QtAutomationMouseClickEvent::clone () const
{
   return new QtAutomationMouseClickEvent ( *this );
}
          
const QPoint & QtAutomationMouseClickEvent::position() const
{
    return m_position;
}

Qt::MouseButton QtAutomationMouseClickEvent::button() const
{
    return m_button;
}

QtAutomationMouseClickEvent::~QtAutomationMouseClickEvent()
{
}
