#include "QtMachineAutomationEvent.h"
#include <sequanto/system.h>

using namespace sequanto::automation;

const int QtMachineAutomationEvent::ID = QEvent::registerEventType();

int QtMachineAutomationEvent::s_eventsPosted = 0;
int QtMachineAutomationEvent::s_totalDeliveryTime = 0;

QtMachineAutomationEvent::QtMachineAutomationEvent ( QtMachineAutomationEvent::Command _command, SQByteArray * _object, int _index )
   : QtAutomationEventWithSynchronization<QVariant> ( (QEvent::Type) ID)
{
   m_command = _command;
   m_index = _index;
   if ( _object != NULL )
   {
      m_object.setRawData ( (const char*) _object->m_start, _object->m_length );
   }
}

bool QtMachineAutomationEvent::normal() const
{
   return m_object.size() == sizeof(QWidget*);
}

QWidget * QtMachineAutomationEvent::widget() const
{
   if ( m_object.isEmpty() )
   {
      return NULL;
   }

   QWidget * widget = NULL;
   memcpy ( &widget, m_object.constData(), sizeof(QWidget*) );
   if ( QApplication::allWidgets().contains(widget) == QBool(true) )
   {
      return widget;
   }
   else
   {
      return NULL;
   }
}

QByteArray QtMachineAutomationEvent::extra() const
{
   QByteArray ret;
   ret.setRawData ( m_object.constData() + sizeof(QWidget*), m_object.length() - sizeof(QWidget*) );
   return ret;
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

const int QtMachineAutomationMouseEvent::ID = QEvent::registerEventType();

QtMachineAutomationMouseEvent::QtMachineAutomationMouseEvent ( QtMachineAutomationMouseEvent::Command _command, int _x, int _y, int _button )
   : QEvent ( (QEvent::Type) ID),
     m_command(_command),
     m_position(_x, _y)
{
   switch ( _button )
   {
   case 0:
      m_button = Qt::LeftButton;
      break;

   case 1:
      m_button = Qt::RightButton;
      break;

   case 2:
      m_button = Qt::MidButton;
      break;
   }
}

QtMachineAutomationMouseEvent::~QtMachineAutomationMouseEvent()
{
}
