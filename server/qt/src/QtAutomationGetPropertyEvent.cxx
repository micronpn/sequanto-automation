#include <sequanto/QtAutomationGetPropertyEvent.h>

using namespace sequanto::automation;

const int QtAutomationGetPropertyEvent::ID = QEvent::registerEventType();

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

    QCoreApplication::postEvent ( _objectToPostEventTo, this );
   
    m_waitCondition.wait ( &m_lock );

    ret = m_value;

    m_lock.unlock();
    m_doneLock.unlock();

    return ret;
}

QtAutomationGetPropertyEvent::~QtAutomationGetPropertyEvent()
{
}
