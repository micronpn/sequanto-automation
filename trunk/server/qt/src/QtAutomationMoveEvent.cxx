#include <sequanto/QtAutomationMoveEvent.h>

using namespace sequanto::automation;

const int QtAutomationMoveEvent::ID = QEvent::registerEventType();

QtAutomationMoveEvent::QtAutomationMoveEvent ( int _x, int _y )
    : QEvent( (QEvent::Type) ID),
      m_position(_x, _y)
{
}
 
const QPoint & QtAutomationMoveEvent::position()
{
    return m_position;
}

QtAutomationMoveEvent::~QtAutomationMoveEvent()
{
}
