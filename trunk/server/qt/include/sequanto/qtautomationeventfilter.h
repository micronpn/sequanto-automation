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

#ifndef SEQUANTO_QT_MOVE_AND_RESIZE_EVENT_FILTER_H_
#define SEQUANTO_QT_MOVE_AND_RESIZE_EVENT_FILTER_H_

#include <string>
#include <map>
#include <sequanto/tree.h>

#include <QObject>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtAutomationMoveEvent : public QEvent
      {
      protected:
         QPoint m_position;

      public:
         static const int ID;

         QtAutomationMoveEvent ( int _x, int _y );

         const QPoint & position ();

         virtual ~QtAutomationMoveEvent ();
      };

      class QtAutomationResizeEvent : public QEvent
      {
      protected:
         QSize m_size;

      public:
         static const int ID;

         QtAutomationResizeEvent ( int _width, int _height );

         const QSize & size ();

         virtual ~QtAutomationResizeEvent ();
      };

      class QtAutomationEventFilter : public QObject
      {
         Q_OBJECT;

      private:
         ListNode * m_node;

      public:
         QtAutomationEventFilter ( ListNode * _node, QObject * _parent );

         virtual bool eventFilter ( QObject * _object, QEvent * _event );
         
         virtual ~QtAutomationEventFilter ();
      };
   }
}

#endif
