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

#ifndef SEQUANTO_QT_AUTOMATION_DELAYED_DONE_EVENT_H_
#define SEQUANTO_QT_AUTOMATION_DELAYED_DONE_EVENT_H_

#include <QObject>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      /**
       * An event which can be "delayed done", meaning that the thread
       * calling wait(QObject*) will wait until done() is called, and
       * that the event can post a copy of itself and when done() is
       * called on the copy, the waiting thread is can continue.
       * 
       * In other words:
       * 
       * It works like QtAutomationEventWithSynchronization, but can
       * be re-posted indefinaetely, this is also why there is no
       * TResult on this class.
       */
      class QtAutomationDelayedDoneEvent : public QEvent
      {
      private:
         QSharedPointer<QMutex> m_lock;
         QSharedPointer<QMutex> m_doneLock;
         QSharedPointer<QWaitCondition> m_waitCondition;
         int m_iteration;
         
      protected:
         QtAutomationDelayedDoneEvent ( const QtAutomationDelayedDoneEvent & _toClone );
         virtual QtAutomationDelayedDoneEvent * clone () const = 0;
         
      public:
         /**
          * Each time the event is delayed, iteration will be
          * incremented by one. So the first time it is handled it
          * will be 0, and the 1, then 2, etc.
          */
         int iteration () const;
         
         QtAutomationDelayedDoneEvent ( QEvent::Type _eventType );
         
         /**
          * Post the event to _objectToSendTo and wait for _timeout
          * milliseconds for the receiving object to call done(). If
          * the wait times out, false will be returned.
          * 
          * This method is static because the event will be deleted
          * during the method call (because it will be handled).
          */
         static bool wait ( QtAutomationDelayedDoneEvent * _event, QObject * _receiver, unsigned long _timeout );
         
         void done ();
         void delayedDone ( QObject * _objectToSendTo );
      };
   }
}

#endif
