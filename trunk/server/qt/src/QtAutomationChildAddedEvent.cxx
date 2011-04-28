#include <sequanto/QtAutomationChildAddedEvent.h>

using namespace sequanto::automation;

const int QtAutomationChildAddedEvent::ID = QEvent::registerEventType();

QtAutomationChildAddedEvent::QtAutomationChildAddedEvent ( QWidget * _child )
   : QEvent( (QEvent::Type) ID),
     m_child(_child)
{
}
 
QWidget * QtAutomationChildAddedEvent::child() const
{
   return m_child;
}

QtAutomationChildAddedEvent::~QtAutomationChildAddedEvent()
{
}
