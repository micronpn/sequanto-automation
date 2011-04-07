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

#ifndef SEQUANTO_READONLY_PROPERTYNODE_H_
#define SEQUANTO_READONLY_PROPERTYNODE_H_

#include <string>
#include <map>
#include <sequanto/macros.h>
#include <sequanto/propertynode.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL ReadOnlyPropertyNode : public PropertyNode
      {
      public:
         ReadOnlyPropertyNode ( const std::string & _name );

         virtual void HandleSet ( const SQValue * const _value );

         virtual ~ReadOnlyPropertyNode ();
      };

      class SQ_DECL ReadOnlyIntegerPropertyNode : public ReadOnlyPropertyNode
      {
      protected:
         virtual int GetValue () = 0;

      public:
         ReadOnlyIntegerPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info() const;

         virtual void HandleGet ( SQValue & _outputValue );

         virtual void SendUpdate ();

         virtual ~ReadOnlyIntegerPropertyNode();
      };

      class SQ_DECL ReadOnlyStringPropertyNode : public ReadOnlyPropertyNode
      {
      protected:
         virtual const std::string & GetValue () = 0;

      public:
         ReadOnlyStringPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info() const;

         virtual void HandleGet ( SQValue & _outputValue );

         virtual void SendUpdate ();

         virtual ~ReadOnlyStringPropertyNode();
      };

      class SQ_DECL ReadOnlyBooleanPropertyNode : public ReadOnlyPropertyNode
      {
      protected:
         virtual bool GetValue () = 0;

      public:
         ReadOnlyBooleanPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info() const;

         virtual void HandleGet ( SQValue & _outputValue );

         virtual void SendUpdate ();

         virtual ~ReadOnlyBooleanPropertyNode();
      };
   }
}

#endif
