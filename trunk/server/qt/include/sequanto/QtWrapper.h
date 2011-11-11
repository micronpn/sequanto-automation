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

#ifndef SEQUANTO_QT_WRAPPER_H_
#define SEQUANTO_QT_WRAPPER_H_

#include <sequanto/macros.h>
#include <sequanto/tree.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtApplicationAutomationEventFilter.h>
#include <sequanto/QtActiveWindowProperty.h>

#include <string>
#include <map>
#include <QtCore>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL QtWrapper
      {
      private:
         QtWrapper ();
         virtual ~QtWrapper ();
         
         friend class QtApplicationAutomationEventFilter;
         friend class QtWidgetAutomationEventFilter;
         friend class QtActiveWindowProperty;
         
         static bool UpdateWindows ();
         static bool UpdateWindows ( ListNode * _windowsNode, QtActiveWindowProperty * _activeWinowNode );
         static bool IsWindow ( QWidget * _widget );
         static void ActiveWindowChanged ();
         
      public:
         static void Wrap ( ListNode * _root, QObject * _object );
         /**
          * Wrap the QWidget and all its children, normally you should
          * use WrapApplication instead.
          * 
          * @see WrapApplication
          */
         static void WrapUi ( QtWidgetNode * _root, QWidget * _widget );
         /**
          * Wrap the QApplication instance in this application. This
          * is the preferred way of wrapping a QT application.
          * 
          * Only one QApplication exists in a single application, 
          */
         static void WrapApplication ( ListNode * _root );

         static std::string ToString ( const QString & _value );
         static std::string GetObjectName ( QObject * _object );

         static QVariant GetPropertyValue ( QObject * _object, const std::string & _propertyName );

         static const std::string & screen_pos ();
         static const std::string & global_pos ();

         static void Log ( const QString & _message );
      };
   }
}

#endif
