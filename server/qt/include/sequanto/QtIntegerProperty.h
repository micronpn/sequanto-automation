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

#ifndef SEQUANTO_QT_INTEGER_PROPERTY_H_
#define SEQUANTO_QT_INTEGER_PROPERTY_H_

#include <sequanto/macros.h>
#include <sequanto/readwritepropertynode.h>
#include <sequanto/QtPropertyChangedNotificationAdapter.h>

namespace sequanto
{
   namespace automation
   {
      class QtIntegerProperty : public IntegerPropertyNode, public virtual IQtPropertyChangedReceiver
      {
      protected:
         QObject * m_object;
         QtPropertyChangedNotificationAdapter * m_notifyAdapter;

         virtual const std::string & QtPropertyName () const;
         
      public:
         QtIntegerProperty ( const std::string & _name, QObject * _object );

         virtual void PropertyChanged ();
         virtual int GetValue ();
         virtual void SetValue ( int _newValue );
         virtual ~QtIntegerProperty();
      };

      class QtIntegerPropertyWithAlternateName : public QtIntegerProperty
      {
      private:
         std::string m_qtPropertyName;

      protected:
         virtual const std::string & QtPropertyName() const;
         
      public:
         QtIntegerPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName );
         QtIntegerPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification  );
         
         void init ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification );
         
         virtual ~QtIntegerPropertyWithAlternateName ();
      };
   }
}

#endif
