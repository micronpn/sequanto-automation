#include "QtMachineAutomationEvent.h"
#include <sequanto/system.h>

using namespace sequanto::automation;

const int QtMachineAutomationEvent::ID = QEvent::registerEventType();

int QtMachineAutomationEvent::s_eventsPosted = 0;
int QtMachineAutomationEvent::s_totalDeliveryTime = 0;

QtMachineAutomationEvent::QtMachineAutomationEvent ( QtMachineAutomationEvent::Command _command, QWidget * _widget, int _index )
   : QtAutomationEventWithSynchronization<QVariant> ( (QEvent::Type) ID)
{
   m_widget = _widget;
   m_command = _command;
   m_index = _index;
}

QWidget * QtMachineAutomationEvent::widget()
{
   if ( QApplication::allWidgets().contains(m_widget) == QBool(true) )
   {
      return m_widget;
   }
   else
   {
      return NULL;
   }
}

void QtMachineAutomationEvent::received()
{
   int deliveryTime = sq_system_tickcount() - m_sentAt;
   
   s_totalDeliveryTime += deliveryTime;
   s_eventsPosted ++;
   
   if ( deliveryTime > 20 )
   {
      //QtWrapper::Log ( QString("Delivery of a GetPropertyEvent for %1 took more than 20 ms.").arg(m_propertyName ) );
   }
}

int QtMachineAutomationEvent::eventsPosted()
{
   return s_eventsPosted;
}

float QtMachineAutomationEvent::averageDeliveryTime()
{
   float ret = s_totalDeliveryTime;
   return (ret / s_eventsPosted);
}

QtMachineAutomationEvent::~QtMachineAutomationEvent()
{
}
