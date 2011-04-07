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

#ifndef SEQUANTO_PROPERTYNODE_H_
#define SEQUANTO_PROPERTYNODE_H_

#include <string>
#include <map>
#include <sequanto/macros.h>
#include <sequanto/node.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL PropertyNode : public sequanto::automation::Node
      {
      public:
         static const NodeInfo & GetIntegerNodeInfo ();
         static const NodeInfo & GetFloatNodeInfo ();
         static const NodeInfo & GetBooleanNodeInfo();
         static const NodeInfo & GetStringNodeInfo();
         static const NodeInfo & GetByteArrayNodeInfo();

         static const NodeInfo & GetReadOnlyIntegerNodeInfo ();
         static const NodeInfo & GetReadOnlyFloatNodeInfo ();
         static const NodeInfo & GetReadOnlyBooleanNodeInfo();
         static const NodeInfo & GetReadOnlyStringNodeInfo();
         static const NodeInfo & GetReadOnlyByteArrayNodeInfo();
         
         PropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info () const = 0;
         virtual void HandleGet ( SQValue & _outputValue ) = 0;
         virtual void HandleSet ( const SQValue * const _value ) = 0;

         virtual void SendUpdate( const SQValue & _value );

         virtual ~PropertyNode ();
      };
   }
}

#endif
