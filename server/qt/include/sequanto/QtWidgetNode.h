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

#ifndef SEQUANTO_QT_WIDGET_NODE_H_
#define SEQUANTO_QT_WIDGET_NODE_H_

#include <sequanto/ui.h>
#include <sequanto/listnode.h>
#include <sequanto/QtWidgetAutomationEventFilter.h>
#include <QObject>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtWidgetNode : public ListNode
      {
      protected:
         friend class QtWidgetAutomationEventFilter;
          
         QWidget * m_widget;
         QtWidgetAutomationEventFilter * m_eventFilter;

         void WidgetDestroyed();

      public:
         QtWidgetNode ( QWidget * _widget );
         
         inline QWidget * widget () const { return m_widget; }
         
         SQWidgetType type () const;
         
         void SendChildrenUpdate ();

		 void SendPositionUpdateForAllChildren();

         typedef enum _AddChildWidgetResult
         {
            NOT_ADDED = 0,
            ADDED = 1,
            ALREADY_EXISTS = 2,
         } AddChildWidgetResult;
         

         /**
          * Add a child widget to the widget contained in this widget node.
          * 
          * @returns true if the child was added to the root, false otherwise (child already added or name is empty/invalid).
          */
         AddChildWidgetResult AddChildWidget ( QWidget * _child );

         //bool RemoveChildWidget ( QWidget * _child );

         void HandleGetNativeId ( SQValue & _value );

         virtual ~QtWidgetNode ();
      };
   }
}

#endif
