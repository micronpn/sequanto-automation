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

#include <string>
#include <map>
#include <sequanto/tree.h>

#include <QtCore>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class QtWrapper
      {
      private:
         QtWrapper ();
         virtual ~QtWrapper ();
         
         friend class QtApplicationAutomationEventFilter;

         static void UpdateWindows ( ListNode * _windows );
	
      public:
         static void Wrap ( ListNode * _root, QObject * _object );
         /**
          * Wrap the QWidget and all its children, normally you should
          * use WrapApplication instead.
          * 
          * @see WrapApplication
          */
         static void WrapUi ( ListNode * _root, QWidget * _widget );
         /**
          * Wrap the QApplication instance in this application. This
          * is the preferred way of wrapping a QT application.
          * 
          * Only one QApplication exists in a single application, 
          */
         static void WrapApplication ( ListNode * _root );
         
         static std::string ToString ( const QString & _value );

	 static QVariant GetPropertyValue ( QObject * _object, const std::string & _propertyName );

	 static const std::string & screen_pos ();
	 static const std::string & global_pos ();
      };
   }
}

#endif
