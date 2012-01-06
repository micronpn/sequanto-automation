#include <sequanto/QtAutomationRefreshWindowsEvent.h>

using namespace sequanto::automation;

const int QtAutomationRefreshWindowsEvent::ID = QEvent::registerEventType();

QtAutomationRefreshWindowsEvent::QtAutomationRefreshWindowsEvent ()
   : QEvent( (QEvent::Type) ID)
{
}
 
QtAutomationRefreshWindowsEvent::~QtAutomationRefreshWindowsEvent()
{
}
