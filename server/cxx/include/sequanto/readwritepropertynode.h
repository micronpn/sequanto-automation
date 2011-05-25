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

#ifndef SEQUANTO_READWRITE_PROPERTYNODE_H_
#define SEQUANTO_READWRITE_PROPERTYNODE_H_

#include <string>
#include <map>
#include <sequanto/macros.h>
#include <sequanto/propertynode.h>
#include <sequanto/readonlypropertynode.h>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL IntegerPropertyNode : public ReadOnlyIntegerPropertyNode
      {
      protected:
         virtual int GetValue () = 0;
         virtual void SetValue ( int _newValue ) = 0;

      public:
         IntegerPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info () const;
         virtual void HandleSet ( const SQValue * const _value );

         virtual ~IntegerPropertyNode();
      };

      class SQ_DECL StringPropertyNode : public ReadOnlyStringPropertyNode
      {
      protected:
         virtual const std::string & GetValue () = 0;
         virtual void SetValue ( const std::string & _newValue ) = 0;

      public:
         StringPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info () const;
         virtual void HandleSet ( const SQValue * const _value );

         virtual ~StringPropertyNode();
      };

      class SQ_DECL BooleanPropertyNode : public ReadOnlyBooleanPropertyNode
      {
      protected:
         virtual bool GetValue () = 0;
         virtual void SetValue ( bool _newValue ) = 0;

      public:
         BooleanPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info () const;
         virtual void HandleSet ( const SQValue * const _value );

         virtual ~BooleanPropertyNode();
      };

      class SQ_DECL FloatPropertyNode : public ReadOnlyFloatPropertyNode
      {
      protected:
         virtual float GetValue () = 0;
         virtual void SetValue ( float _newValue ) = 0;

      public:
         FloatPropertyNode ( const std::string & _name );

         virtual const NodeInfo & Info () const;
         virtual void HandleSet ( const SQValue * const _value );

         virtual ~FloatPropertyNode();
      };
   }
}

#endif
