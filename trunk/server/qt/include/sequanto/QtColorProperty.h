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

#ifndef SEQUANTO_QT_COLOR_PROPERTY_H_
#define SEQUANTO_QT_COLOR_PROPERTY_H_

#include <sequanto/macros.h>
#include <sequanto/propertynode.h>
#include <sequanto/QtPropertyChangedNotificationAdapter.h>
#include <QtGui>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL QtColorProperty : public PropertyNode, public virtual IQtPropertyChangedReceiver
      {
      protected:
         QWidget * m_widget;
         QPalette::ColorRole m_role;
         
      public:
         QtColorProperty ( const std::string & _name, QWidget * _widget, QPalette::ColorRole _role );
         
         virtual const NodeInfo & Info () const;
         virtual void PropertyChanged ();
         virtual void HandleGet ( SQValue & _outputValue );
         virtual void HandleSet ( const SQValue * const _value );
         virtual ~QtColorProperty();
      };
   }
}

#endif
