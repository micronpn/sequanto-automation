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

#ifndef SEQUANTO_QT_AUTOMATION_STEAL_FOCUS_EVENT_H_
#define SEQUANTO_QT_AUTOMATION_STEAL_FOCUS_EVENT_H_

#include <QObject>
#include <QtGui>
#include <sequanto/QtAutomationDelayedDoneEvent.h>

namespace sequanto
{
   namespace automation
   {
      class QtAutomationStealFocusEvent : public QtAutomationDelayedDoneEvent
      {
      protected:
         virtual QtAutomationStealFocusEvent * clone () const;
         
      public:
         static const int ID;
         
         QtAutomationStealFocusEvent ();
         void stealFocus();
         virtual ~QtAutomationStealFocusEvent ();
      };
   }
}

#endif
