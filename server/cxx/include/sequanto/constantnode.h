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

#ifndef SEQUANTO_CONSTANTNODE_H_
#define SEQUANTO_CONSTANTNODE_H_

#include <string>
#include <map>
#include <sequanto/propertynode.h>
#include <sequanto/readonlypropertynode.h>
#include <sequanto/readwritepropertynode.h>

namespace sequanto
{
   namespace automation
   {
      class ConstantIntegerNode : public ReadOnlyIntegerPropertyNode
      {
      private:
         int m_value;
         
      protected:
         virtual int GetValue ();

      public:
         ConstantIntegerNode ( const std::string & _name, int _value );

         virtual ~ConstantIntegerNode();
      };

      class ConstantStringNode : public ReadOnlyStringPropertyNode
      {
      private:
         std::string m_value;
         
      protected:
         virtual const std::string & GetValue ();

      public:
         ConstantStringNode ( const std::string & _name, const char * _value );
         ConstantStringNode ( const std::string & _name, const std::string & _value );

         virtual ~ConstantStringNode();
      };

      class ConstantBooleanNode : public ReadOnlyBooleanPropertyNode
      {
      private:
         bool m_value;
         
      protected:
         virtual bool GetValue ();

      public:
         ConstantBooleanNode ( const std::string & _name, bool _value );

         virtual ~ConstantBooleanNode();
      };
   }
}

#endif
