#include <sequanto/QtAutomationMouseMoveEvent.h>

using namespace sequanto::automation;

const int QtAutomationMouseMoveEvent::ID = QEvent::registerEventType();

QtAutomationMouseMoveEvent::QtAutomationMouseMoveEvent ( int _x, int _y )
    : QEvent( (QEvent::Type) ID),
      m_position(_x, _y)
{
}
 
const QPoint & QtAutomationMouseMoveEvent::position()
{
    return m_position;
}

QtAutomationMouseMoveEvent::~QtAutomationMouseMoveEvent()
{
}
