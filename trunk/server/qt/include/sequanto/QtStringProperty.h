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

#ifndef SEQUANTO_QT_STRING_PROPERTY_H_
#define SEQUANTO_QT_STRING_PROPERTY_H_

#include <sequanto/macros.h>
#include <sequanto/propertynode.h>
#include <sequanto/QtPropertyChangedNotificationAdapter.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL QtStringProperty : public PropertyNode, public virtual IQtPropertyChangedReceiver
      {
      protected:
         QObject * m_object;

         QtPropertyChangedNotificationAdapter * m_notifyAdapter;

         virtual const std::string & QtPropertyName () const;

      public:
         QtStringProperty ( const std::string & _name, QObject * _object );
   
         virtual const NodeInfo & Info () const;
         virtual void PropertyChanged ();
         virtual void HandleGet ( SQValue & _outputValue );
         virtual void HandleSet ( const SQValue * const _value );
         virtual ~QtStringProperty();
      };

      class QtStringPropertyWithAlternateName : public QtStringProperty
      {
      private:
         std::string m_qtPropertyName;

      protected:
         virtual const std::string & QtPropertyName() const;
         
      public:
         QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName );
         QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification  );
         
         void init ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification );
         
         virtual ~QtStringPropertyWithAlternateName ();
      };
   }
}

#endif
