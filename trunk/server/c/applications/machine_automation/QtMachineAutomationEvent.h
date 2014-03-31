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

#ifndef SEQUANTO_QT_MACHINE_AUTOMATION__EVENT_H_
#define SEQUANTO_QT_MACHINE_AUTOMATION_EVENT_H_

#include <QObject>
#include <QtGui>
#include <sequanto/value.h>
#include "QtAutomationEventWithSynchronization.h"

namespace sequanto
{
   namespace automation
   {
      class QtMachineAutomationEvent : public QtAutomationEventWithSynchronization<QVariant>
      {
      public:
         enum Command
         {
            DESKTOP0,
            NAME,
            X,
            Y,
            WIDTH,
            HEIGHT,
            ROLE,
            TEXT,
            ACTIONS,
            ACTION_NAME,
            ACTION_DO,
            CHILDREN,
            CHILD,
         };

      protected:
         Command m_command;
         QByteArray m_object;
         int m_index;

         static int s_eventsPosted;
         static int s_totalDeliveryTime;

      public:
         static const int ID;

         QtMachineAutomationEvent ( Command _command, SQByteArray * _object, int _index );

         inline Command command () const { return m_command; }
         inline int index() const { return m_index; }
         inline const QByteArray & object() const { return m_object; }
         
         bool normal() const;
         QWidget * widget() const;
         QByteArray extra() const;
         
         void received ();

         virtual ~QtMachineAutomationEvent ();

         static int eventsPosted ();
         static float averageDeliveryTime ();
      };

      class QtMachineAutomationMouseEvent : public QEvent
      {
      private:
         QPoint m_position;
         Qt::MouseButton m_button;

      public:
         static const int ID;

         inline const QPoint position () const { return m_position; }
         inline Qt::MouseButton button () const { return m_button; }

         QtMachineAutomationMouseEvent ( int _x, int _y, int _button );

         virtual ~QtMachineAutomationMouseEvent();
      };
   }
}

#endif
