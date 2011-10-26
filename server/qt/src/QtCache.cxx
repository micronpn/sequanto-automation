#include <sequanto/system.h>
#include <sequanto/lock.h>
#include <sequanto/QtCache.h>
#include <sequanto/QtUpdateCacheEvent.h>
#include <iostream>

using namespace sequanto::automation;

QtCache::QtCache ()
{
   m_cacheHits = 0;
   m_cacheMisses = 0;
   
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
   if ( _object->thread() == QThread::currentThread() )
   {
      QtCacheItem::getDirectly ( _object, _property, _outputValue );
   }
   else
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
               //std::cout << "Expiring cache for " << m_cache[i]->m_object << " (age is " << (cacheTimeout - m_cache[i]->m_time) << ")" << std::endl;
               delete m_cache[i];
               m_cache[i] = NULL;
               emptyIndex = i;
            }
            else
            {
               if ( m_cache[i]->m_object == _object )
               {
                  m_cacheHits ++;
                  //std::cout << "Found cache for " << _object << std::endl;
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
   
      //std::cout << "Cache for " << _object << " not found. Creating at " << emptyIndex << std::endl;
      m_cacheMisses ++;
   
      m_cache[emptyIndex] = new QtCacheItem(_object);
      QtUpdateCacheEvent * event = new QtUpdateCacheEvent(m_cache[emptyIndex]);
      event->wait(_object);
      sq_value_copy ( &m_cache[emptyIndex]->m_cachedValues[_property], &_outputValue );
   }
}

int QtCache::cacheHits () const
{
   return m_cacheHits;
}

int QtCache::cacheMisses () const
{
   return m_cacheMisses;
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

void QtCacheItem::getDirectly ( QWidget * _object, QtCacheItem::Property _property, SQValue & _output )
{
   switch ( _property )
   {
   case GLOBAL_X:
   case GLOBAL_Y:
      {
         QWidget * window = _object->window();
         QPoint pos = _object->mapToGlobal(QPoint(0,0));
         QPoint windowTopLeft ( window->geometry().topLeft() );
         
         pos -= windowTopLeft;
         
         if ( _property == GLOBAL_X )
         {
            sq_value_integer ( &_output, pos.x() );
         }
         else
         {
            sq_value_integer ( &_output, pos.y() );
         }
      }
      break;
      
   case WIDTH:
      sq_value_integer ( &_output, _object->width() );
      break;
      
   case HEIGHT:
      sq_value_integer ( &_output, _object->height() );
      break;

   case ENABLED:
      sq_value_boolean ( &_output, _object->isEnabled() );
      break;
      
   case VISIBLE:
      sq_value_boolean ( &_output, _object->isVisible() );
      break;
      
   default:
      break;
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
   
   getDirectly ( m_object, WIDTH, m_cachedValues[WIDTH] );
   getDirectly ( m_object, HEIGHT, m_cachedValues[HEIGHT] );
   getDirectly ( m_object, ENABLED, m_cachedValues[ENABLED] );
   getDirectly ( m_object, VISIBLE, m_cachedValues[VISIBLE] );
}

QtCacheItem::~QtCacheItem ()
{
   for ( int i = 0; i < NUMBER_OF_PROPERTIES; i++ )
   {
      sq_value_free ( &m_cachedValues[i] );
   }
   m_object = NULL;
}
