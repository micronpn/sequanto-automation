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

#ifndef SEQUANTO_QT_STATS_PROPERTIES_H_
#define SEQUANTO_QT_STATS_PROPERTIES_H_

#include <sequanto/readonlypropertynode.h>

namespace sequanto
{
   namespace automation
   {
      class QtStatsGetPropertyEventsHandled : public ReadOnlyIntegerPropertyNode
      {
      protected:
         virtual int GetValue();
         
      public:
         QtStatsGetPropertyEventsHandled ();
      };

      class QtStatsGetPropertyAverageDeliveryTime : public ReadOnlyFloatPropertyNode
      {
      protected:
         virtual float GetValue();
         
      public:
         QtStatsGetPropertyAverageDeliveryTime ();
      };
   }
}

#endif
