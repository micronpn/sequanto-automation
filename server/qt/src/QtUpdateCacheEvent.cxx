#include <sequanto/QtUpdateCacheEvent.h>
#include <cassert>

using namespace sequanto::automation;

const int QtUpdateCacheEvent::ID = QEvent::registerEventType();

QtUpdateCacheEvent::QtUpdateCacheEvent ( QtCacheItem * _cacheItem )
   : QEvent( (QEvent::Type) ID),
     m_cacheItem ( _cacheItem )
{
}

void QtUpdateCacheEvent::wait ( QObject * _object )
{
   assert ( _object == m_cacheItem->m_object );
   
   if ( _object->thread() == QThread::currentThread() )
   {
      m_cacheItem->update();
   }
   else
   {
      m_doneLock.lock();
      m_lock.lock();
      
      QCoreApplication::postEvent ( _object, this );
      
      m_waitCondition.wait ( &m_lock );
      
      m_lock.unlock();
      m_doneLock.unlock();
   }
}

void QtUpdateCacheEvent::update ()
{
   m_lock.lock();
   
   m_cacheItem->update();
   
   m_waitCondition.wakeAll ();
   
   m_lock.unlock();

   m_doneLock.lock ();
   
   m_doneLock.unlock ();
}

QtUpdateCacheEvent::~QtUpdateCacheEvent ()
{
}
