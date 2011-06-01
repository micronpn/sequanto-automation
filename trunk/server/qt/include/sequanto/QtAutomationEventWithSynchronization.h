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

#ifndef SEQUANTO_QT_AUTOMATION_EVENT_WITH_SYNCHRONIZATION_H_
#define SEQUANTO_QT_AUTOMATION_EVENT_WITH_SYNCHRONIZATION_H_

#include <QObject>
#include <QtGui>
#include <sequanto/QtAutomationDelayedDoneEvent.h>
#include <sequanto/system.h>

namespace sequanto
{
   namespace automation
   {
      /**
       * Abstract base class for events which are run on the UI thread
       * (by being posted using QApplication::postEvent), but which
       * needs to be synchronized with the caller (the caller must
       * wait for the event to be handled before continuing.
       */
      template <typename TResult>
      class QtAutomationEventWithSynchronization : public QEvent
      {
      private:
         QMutex m_lock;
         QMutex m_doneLock;
         QWaitCondition m_waitCondition;
         TResult m_result;

      protected:
         int m_sentAt;
         
      public:
         QtAutomationEventWithSynchronization ( QEvent::Type _eventId )
            : QEvent ( _eventId )
         {
         }
         
         TResult wait( QObject * _objectToPostEventTo )
         {
            m_doneLock.lock();
            m_lock.lock();
            
            m_sentAt = sq_system_tickcount();
            
            QCoreApplication::postEvent ( _objectToPostEventTo, this );

            m_waitCondition.wait ( &m_lock );

            TResult ret = m_result;
            
            m_lock.unlock();
            m_doneLock.unlock();
            
            return ret;
         }
         
         void done ( const TResult & _result )
         {
            m_lock.lock();
            
            m_result = _result;
            
            m_waitCondition.wakeAll ();
            
            m_lock.unlock();
            
            m_doneLock.lock ();
            
            m_doneLock.unlock ();
         }
         
         /**
          * Post a new message to the _receiver object's event queue,
          * which will mark the event as done when that message is
          * processed.
          *
          * This can be used when finishing an event requires other
          * events to be processed before, for example when processing
          * a mouse move event which means that the move events must
          * be processed before the event is actually done.
          *
         void delayedDone ( QObject * _receiver )
         {
            QApplication::postEvent ( _receiver, new QtAutomationDelayedDoneEvent(m_lock, m_doneLock, m_waitCondition) );
            
            m_lock = NULL;
            m_doneLock = NULL;
            m_waitCondition = NULL;
         }
         */
      };
   }
}

#endif
