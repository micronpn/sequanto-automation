#include <sequanto/QtAutomationDelayedDoneEvent.h>

using namespace sequanto::automation;

QtAutomationDelayedDoneEvent::QtAutomationDelayedDoneEvent ( QEvent::Type _eventType )
   : QEvent ( _eventType ),
     m_lock ( new QMutex() ),
     m_doneLock ( new QMutex() ),
     m_waitCondition ( new QWaitCondition() ),
     m_iteration ( 0 )
{
}

QtAutomationDelayedDoneEvent::QtAutomationDelayedDoneEvent ( const QtAutomationDelayedDoneEvent & _event )
   : QEvent ( _event.type() ),
     m_lock ( _event.m_lock ),
     m_doneLock ( _event.m_doneLock ),
     m_waitCondition ( _event.m_waitCondition ),
     m_iteration ( _event.m_iteration + 1 )
{
}

int QtAutomationDelayedDoneEvent::iteration () const
{
   return m_iteration;
}

bool QtAutomationDelayedDoneEvent::wait ( QtAutomationDelayedDoneEvent * _event, QObject * _objectToSendTo, unsigned long _timeout )
{
   QSharedPointer<QMutex> lock ( _event->m_lock );
   QSharedPointer<QMutex> doneLock ( _event->m_doneLock );
   QSharedPointer<QWaitCondition> waitCondition ( _event->m_waitCondition );
   
   Q_ASSERT ( !lock.isNull() );
   Q_ASSERT ( !doneLock.isNull() );
   Q_ASSERT ( !waitCondition.isNull() );
   
   doneLock->lock();
   lock->lock();
   
   QCoreApplication::postEvent ( _objectToSendTo, _event );
   
   bool ret = waitCondition->wait ( lock.data(), _timeout );
   
   lock->unlock();
   doneLock->unlock();
   
   return ret;
}

void QtAutomationDelayedDoneEvent::delayedDone ( QObject * _objectToSendTo )
{
   //qDebug () << "QtAutomationDelayedDoneEvent::delayed done";
   
   QApplication::postEvent ( _objectToSendTo, clone() );
}

void QtAutomationDelayedDoneEvent::done ()
{
   //qDebug () << "QtAutomationDelayedDoneEvent::done";
   
   m_lock->lock();
   
   //qDebug () << "QtAutomationDelayedDoneEvent::waking all";
   
   m_waitCondition->wakeAll ();

   m_lock->unlock();

   //qDebug () << "QtAutomationDelayedDoneEvent::locking done lock";
   
   m_doneLock->lock ();
   
   m_doneLock->unlock ();

   //qDebug () << "QtAutomationDelayedDoneEvent::done lock unlocked";
}
