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

#ifndef SEQUANTO_QT_SCROLL_BAR_PROPERTY_H_
#define SEQUANTO_QT_SCROLL_BAR_PROPERTY_H_

#include <sequanto/QtIntegerProperty.h>
#include <sequanto/QtWidgetNode.h>
#include <QtCore>

namespace sequanto
{
   namespace automation
   {
      class QtScrollBarProperty : public QtIntegerProperty
      {
      private:
         QtWidgetNode * m_associatedScrollArea;
         bool m_hasSearchedForAssociatedScrollArea;
   
      public:
         QtScrollBarProperty ( const std::string & _name, QObject * _object );
   
         virtual void PropertyChanged ();
   
         ~QtScrollBarProperty();
      };
   }
}

#endif
