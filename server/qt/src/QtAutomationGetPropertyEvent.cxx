#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/system.h>

using namespace sequanto::automation;

const int QtAutomationGetPropertyEvent::ID = QEvent::registerEventType();

int QtAutomationGetPropertyEvent::s_eventsPosted = 0;
int QtAutomationGetPropertyEvent::s_totalDeliveryTime = 0;

QtAutomationGetPropertyEvent::QtAutomationGetPropertyEvent ( const char * const _propertyName )
    : QEvent( (QEvent::Type) ID),
      m_propertyName(_propertyName)
{
}
 
const QVariant & QtAutomationGetPropertyEvent::value() const
{
    return m_value;
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

void QtAutomationGetPropertyEvent::done( const QVariant & _value )
{
    m_lock.lock();

    m_value = _value;
  
    m_waitCondition.wakeAll ();

    m_lock.unlock();

    m_doneLock.lock ();

    m_doneLock.unlock ();
}

QVariant QtAutomationGetPropertyEvent::wait(QObject * _objectToPostEventTo )
{
    QVariant ret;

    m_doneLock.lock();
    m_lock.lock();
    
    m_sentAt = sq_system_tickcount ();
    
    QCoreApplication::postEvent ( _objectToPostEventTo, this );
   
    m_waitCondition.wait ( &m_lock );

    ret = m_value;

    m_lock.unlock();
    m_doneLock.unlock();

    return ret;
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
