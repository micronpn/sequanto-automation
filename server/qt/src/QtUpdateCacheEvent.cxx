#include <sequanto/QtUpdateCacheEvent.h>
#include <cassert>

using namespace sequanto::automation;

const int QtUpdateCacheEvent::ID = QEvent::registerEventType();

QtUpdateCacheEvent::QtUpdateCacheEvent ( QtCacheItem * _cacheItem )
   : QtAutomationEventWithSynchronization<bool>( (QEvent::Type) ID),
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
      QtAutomationEventWithSynchronization<bool>::wait ( _object );
   }
}

void QtUpdateCacheEvent::update ()
{
   m_cacheItem->update();
}

QtUpdateCacheEvent::~QtUpdateCacheEvent ()
{
}
