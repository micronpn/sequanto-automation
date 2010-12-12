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

#ifndef SEQUANTO_QT_WIDGET_AUTOMATION_EVENT_FILTER_H_
#define SEQUANTO_QT_WIDGET_AUTOMATION_EVENT_FILTER_H_

#include <QObject>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtWidgetNode;
      
      class QtWidgetAutomationEventFilter : public QObject
      {
         Q_OBJECT;

      private:
         QtWidgetNode * m_node;

      public:
         QtWidgetAutomationEventFilter ( QtWidgetNode * _node );

         virtual bool eventFilter ( QObject * _object, QEvent * _event );
         
         virtual ~QtWidgetAutomationEventFilter ();
      };
   }
}

#endif
