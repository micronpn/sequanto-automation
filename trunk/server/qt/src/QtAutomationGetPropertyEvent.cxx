#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/system.h>

using namespace sequanto::automation;

const int QtAutomationGetPropertyEvent::ID = QEvent::registerEventType();

int QtAutomationGetPropertyEvent::s_eventsPosted = 0;
int QtAutomationGetPropertyEvent::s_totalDeliveryTime = 0;

QtAutomationGetPropertyEvent::QtAutomationGetPropertyEvent ( const char * const _propertyName )
   : QtAutomationEventWithSynchronization<QVariant> ( (QEvent::Type) ID),
     m_propertyName(_propertyName)
{
}
 
const char * QtAutomationGetPropertyEvent::propertyName() const
{
    return m_propertyName;
}

void QtAutomationGetPropertyEvent::received()
{
   int deliveryTime = sq_system_tickcount() - m_sentAt;
   
   s_totalDeliveryTime += deliveryTime;
   s_eventsPosted ++;
   
   if ( deliveryTime > 20 )
   {
      QtWrapper::Log ( QString("Delivery of a GetPropertyEvent for %1 took more than 20 ms.").arg(m_propertyName ) );
   }
}

int QtAutomationGetPropertyEvent::eventsPosted()
{
   return s_eventsPosted;
}

float QtAutomationGetPropertyEvent::averageDeliveryTime()
{
   float ret = s_totalDeliveryTime;
   return (ret / s_eventsPosted);
}

QtAutomationGetPropertyEvent::~QtAutomationGetPropertyEvent()
{
}
