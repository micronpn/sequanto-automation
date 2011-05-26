/*
 * Copyright 2010 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 */

#ifndef SEQUANTO_QT_CACHE_H_
#define SEQUANTO_QT_CACHE_H_

#include <QtGui>
#include <sequanto/value.h>
#include <sequanto/stream.h>
#include <sequanto/mutex.h>

namespace sequanto
{
   namespace automation
   {
      class QtCacheItem
      {
      public:
         typedef enum _Property
         {
            GLOBAL_X = 0,
            GLOBAL_Y,
            WIDTH,
            HEIGHT,
            ENABLED,
            VISIBLE,
            NUMBER_OF_PROPERTIES,
            UNCACHEABLE_PROPERTY,
         } Property;
         
         friend class QtCache;
         friend class QtUpdateCacheEvent;
         
      private:
         QWidget * m_object;
         int m_time;
         SQValue m_cachedValues[NUMBER_OF_PROPERTIES];
         
      public:
         QtCacheItem ( QWidget * _object );
         void update ();
         ~QtCacheItem ();
      };

      class QtCache
      {
      private:
         static const int CACHE_TIMEOUT = 1000;
         static const int CACHE_SIZE = 10;
         QtCacheItem * m_cache[CACHE_SIZE];
         Mutex m_mutex;
         int m_cacheMisses;
         int m_cacheHits;
         
      public:
         static QtCache & Instance();
         
         QtCache ();
         void HandleGet ( QWidget * _object, QtCacheItem::Property _property, SQValue & _outputValue );
         int cacheHits () const;
         int cacheMisses () const;
         ~QtCache ();
      };
   }
}

#endif
