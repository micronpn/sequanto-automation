#include <sequanto/QtAutomationWidgetCreatedEvent.h>

using namespace sequanto::automation;

const int QtAutomationWidgetCreatedEvent::ID = QEvent::registerEventType();

QtAutomationWidgetCreatedEvent::QtAutomationWidgetCreatedEvent ( QWidget * _widget )
   : QEvent( (QEvent::Type) ID),
     m_widget(_widget)
{
}
 
QWidget * QtAutomationWidgetCreatedEvent::widget() const
{
   return m_widget;
}

QtAutomationWidgetCreatedEvent::~QtAutomationWidgetCreatedEvent()
{
}
