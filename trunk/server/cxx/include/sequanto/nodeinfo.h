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

#ifndef SEQUANTO_NODEINFO_H_
#define SEQUANTO_NODEINFO_H_

#include <string>
#include <sequanto/value.h>

namespace sequanto
{
   namespace automation
   {
      class NodeInfo
      {
      public:
         typedef enum _Type
         {
            SQ_NODE_TYPE_LIST = 0,
            SQ_NODE_TYPE_READABLE_PROPERTY = 1,
            SQ_NODE_TYPE_WRITABLE_PROPERTY = 2,
            SQ_NODE_TYPE_READ_WRITE_PROPERTY = 4,
            SQ_NODE_TYPE_CALLABLE = 8,
            SQ_NODE_TYPE_MONITOR = 16,

            SQ_NODE_TYPE_ANY_READABLE_PROPERTY = SQ_NODE_TYPE_READABLE_PROPERTY | SQ_NODE_TYPE_READ_WRITE_PROPERTY,
            SQ_NODE_TYPE_ANY_WRITABLE_PROPERTY = SQ_NODE_TYPE_WRITABLE_PROPERTY | SQ_NODE_TYPE_READ_WRITE_PROPERTY,
            SQ_NODE_TYPE_ANY_PROPERTY = SQ_NODE_TYPE_READABLE_PROPERTY | SQ_NODE_TYPE_WRITABLE_PROPERTY | SQ_NODE_TYPE_READ_WRITE_PROPERTY,
         } Type;

         NodeInfo ();

         virtual Type GetType () const = 0;
         virtual SQValueType GetReturnType () const;
         virtual size_t GetNumberOfParameters () const;
         virtual SQValueType GetParameterType ( size_t _parameterIndex ) const;
         virtual ~NodeInfo ();
      };
   }
}

#endif
