#include <sequanto/QtAutomationMouseClickEvent.h>

using namespace sequanto::automation;

const int QtAutomationMouseClickEvent::ID = QEvent::registerEventType();

QtAutomationMouseClickEvent::QtAutomationMouseClickEvent ( int _x, int _y, Qt::MouseButton _button )
    : QEvent( (QEvent::Type) ID),
      m_position(_x, _y),
      m_button(_button)
{
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
