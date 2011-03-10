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

#ifndef SEQUANTO_QT_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_H_
#define SEQUANTO_QT_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_H_

#include <QObject>

namespace sequanto
{
   namespace automation
   {
      class IQtPropertyChangedReceiver
      {
      public:
         virtual void PropertyChanged () = 0;
      };
      
      class QtPropertyChangedNotificationAdapter : public QObject
      {
         Q_OBJECT;
         
      private:
         QObject * m_object;
         std::string m_methodName;
         IQtPropertyChangedReceiver * m_receiver;

      private slots:
         void propertyChanged ();
         
      public:
         QtPropertyChangedNotificationAdapter ( IQtPropertyChangedReceiver * _receiver, QObject * _object, const std::string & _methodName );

         virtual ~QtPropertyChangedNotificationAdapter ();

         static QtPropertyChangedNotificationAdapter * ConnectIfPossible ( QObject * _object, const std::string & _propertyName, IQtPropertyChangedReceiver * _receiver );
      };
   }
}

#endif
