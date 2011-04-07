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

#ifndef SEQUANTO_METHODINFO_H_
#define SEQUANTO_METHODINFO_H_

#include <vector>
#include <sequanto/macros.h>
#include <sequanto/node.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL MethodInfo : public NodeInfo
      {
      protected:
         SQValueType m_returnType;
         std::vector<SQValueType> m_parameters;
         
      public:
         MethodInfo ();
         MethodInfo ( SQValueType _returnType );
         
         virtual Type GetType () const;
         virtual SQValueType GetReturnType () const;
         virtual size_t GetNumberOfParameters () const;
         virtual SQValueType GetParameterType ( size_t _parameterIndex ) const;
         virtual ~MethodInfo ();

         void SetReturnType ( SQValueType _type );
         void AddParameter ( SQValueType _type );
      };
   }
}

#endif
