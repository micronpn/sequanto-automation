#include <sequanto/QtAutomationChildAddedEvent.h>

using namespace sequanto::automation;

const int QtAutomationChildAddedEvent::ID = QEvent::registerEventType();

QtAutomationChildAddedEvent::QtAutomationChildAddedEvent ( QWidget * _child, int _iteration )
   : QEvent( (QEvent::Type) ID),
     m_child(_child),
     m_iteration(_iteration)
{
}
 
QWidget * QtAutomationChildAddedEvent::child() const
{
   return m_child;
}

int QtAutomationChildAddedEvent::iteration() const
{
   return m_iteration;
}

QtAutomationChildAddedEvent::~QtAutomationChildAddedEvent()
{
}
