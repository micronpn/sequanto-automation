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
 */

#ifndef SEQUANTO_QT_CACHED_PROPERTY_H_
#define SEQUANTO_QT_CACHED_PROPERTY_H_

#include <sequanto/macros.h>
#include <sequanto/QtStringProperty.h>
#include <sequanto/QtIntegerProperty.h>
#include <sequanto/QtBooleanProperty.h>
#include <sequanto/QtCache.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL QtCachedStringProperty : public QtStringProperty
      {
      private:
         QtCacheItem::Property m_cacheProperty;

      public:
         QtCachedStringProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty );
         virtual void HandleGet ( SQValue & _outputValue );
         virtual ~QtCachedStringProperty();
      };

      class SQ_DECL QtCachedIntegerProperty : public QtIntegerProperty
      {
      private:
         QtCacheItem::Property m_cacheProperty;

      public:
         QtCachedIntegerProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty );
         virtual void HandleGet ( SQValue & _outputValue );
         virtual ~QtCachedIntegerProperty();
      };

      class SQ_DECL QtCachedBooleanProperty : public QtBooleanProperty
      {
      private:
         QtCacheItem::Property m_cacheProperty;

      public:
         QtCachedBooleanProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty );
         virtual void HandleGet ( SQValue & _outputValue );
         virtual ~QtCachedBooleanProperty();
      };
   }
}

#endif
