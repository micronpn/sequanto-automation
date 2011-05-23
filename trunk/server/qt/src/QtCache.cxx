#include <sequanto/system.h>
#include <sequanto/lock.h>
#include <sequanto/QtCache.h>
#include <sequanto/QtUpdateCacheEvent.h>
#include <iostream>

using namespace sequanto::automation;

QtCache::QtCache ()
{
   for ( int i = 0; i < CACHE_SIZE; i++ )
   {
      m_cache[i] = NULL;
   }
}

QtCache & QtCache::Instance()
{
   static QtCache s_cache;
   
   return s_cache;
}

void QtCache::HandleGet ( QWidget * _object, QtCacheItem::Property _property, SQValue & _outputValue )
{
   Lock lock ( m_mutex );
   
   int cacheTimeout = sq_system_tickcount() - CACHE_TIMEOUT;
   int emptyIndex = -1;
   for ( int i = 0; i < CACHE_SIZE; i++ )
   {
      if ( m_cache[i] == NULL )
      {
         emptyIndex = i;
      }
      else
      {
         if ( m_cache[i]->m_time < cacheTimeout )
         {
            std::cout << "Expiring cache for " << m_cache[i]->m_object << " (age is " << (cacheTimeout - m_cache[i]->m_time) << ")" << std::endl;
            delete m_cache[i];
            m_cache[i] = NULL;
            emptyIndex = i;
         }
         else
         {
            if ( m_cache[i]->m_object == _object )
            {
               std::cout << "Found cache for " << _object << std::endl;
               sq_value_copy ( &m_cache[i]->m_cachedValues[_property], &_outputValue );
               return;
            }
         }
      }
   }
   
   // Cached value not found (or cache too old), update the cache
   if ( emptyIndex == -1 )
   {
      // TODO: Find oldest cached item
      delete m_cache[0];
      m_cache[0] = NULL;
      emptyIndex = 0;
   }
   
   std::cout << "Cache for " << _object << " not found. Creating at " << emptyIndex << std::endl;
   
   m_cache[emptyIndex] = new QtCacheItem(_object);
   QtUpdateCacheEvent * event = new QtUpdateCacheEvent(m_cache[emptyIndex]);
   event->wait(_object);
   sq_value_copy ( &m_cache[emptyIndex]->m_cachedValues[_property], &_outputValue );
}

QtCache::~QtCache()
{
   for ( int i = 0; i < CACHE_SIZE; i++ )
   {
      if ( m_cache[i] != NULL )
      {
         delete m_cache[i];
         m_cache[i] = NULL;
      }
   }
}

QtCacheItem::QtCacheItem ( QWidget * _object )
   : m_object(_object),
     m_time(sq_system_tickcount())
{
   for ( int i = 0; i < NUMBER_OF_PROPERTIES; i++ )
   {
      sq_value_init ( &m_cachedValues[i] );
   }
}

void QtCacheItem::update ()
{
   m_time = sq_system_tickcount();

   QWidget * window = m_object->window();
   QPoint pos = m_object->mapToGlobal(QPoint(0,0));
   QPoint windowTopLeft ( window->geometry().topLeft() );
   
   pos -= windowTopLeft;
   
   sq_value_integer ( &m_cachedValues[GLOBAL_X], pos.x() );
   sq_value_integer ( &m_cachedValues[GLOBAL_Y], pos.y() );
   sq_value_integer ( &m_cachedValues[WIDTH], m_object->width() );
   sq_value_integer ( &m_cachedValues[HEIGHT], m_object->height() );
   sq_value_boolean ( &m_cachedValues[ENABLED], m_object->isEnabled() );
   sq_value_boolean ( &m_cachedValues[VISIBLE], m_object->isVisible() );
}

QtCacheItem::~QtCacheItem ()
{
   for ( int i = 0; i < NUMBER_OF_PROPERTIES; i++ )
   {
      sq_value_free ( &m_cachedValues[i] );
   }
   m_object = NULL;
}
