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

#ifndef SEQUANTO_QT_AUTOMATION_GET_PROPERTY_EVENT_H_
#define SEQUANTO_QT_AUTOMATION_GET_PROPERTY_EVENT_H_

#include <QObject>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtAutomationGetPropertyEvent : public QEvent
      {
      protected:
         const char * const m_propertyName;
         QVariant m_value;
         QMutex m_lock;
         QMutex m_doneLock;
         QWaitCondition m_waitCondition;
         int m_sentAt;
         static int s_eventsPosted;
         static int s_totalDeliveryTime;
         
      public:
         static const int ID;
         
         QtAutomationGetPropertyEvent ( const char * const _propertyName );
         
         const QVariant & value () const;
         const char * propertyName() const;
         void received ();
         void done ( const QVariant & _value );
         QVariant wait( QObject * _objectToPostEventTo );
         
         virtual ~QtAutomationGetPropertyEvent ();

         static int eventsPosted ();
         static float averageDeliveryTime ();
      };
   }
}

#endif
