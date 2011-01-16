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

#ifndef SEQUANTO_NODE_H_
#define SEQUANTO_NODE_H_

#include <string>

#include <sequanto/nodeinfo.h>

namespace sequanto
{
   namespace automation
   {
#include <sequanto/value.h>

      class ListNode;

      class Node
      {
      private:
         friend class ListNode;

         Node * m_parent;
         std::string m_name;
         std::string m_fullname;

         void SetParent ( Node * _parent );

      protected:
         Node ( const std::string & _name );

      public:
         class Iterator
         {
         public:
            virtual bool HasNext () const = 0;
            virtual Node * GetCurrent () const = 0;
            virtual void Next () = 0;
            virtual ~Iterator();
         };

         virtual Node * GetParent () const;
         virtual const std::string & GetName () const;
         virtual const std::string & GetFullName ();
         virtual const NodeInfo & Info () const = 0;

         virtual Node * FindChild ( const std::string & _name ) const;
         virtual Iterator * ListChildren () const;

         virtual void HandleGet ( SQValue & _outputValue );
         virtual void HandleSet ( const SQValue * const _value );
         virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output );
         virtual void HandleMonitorStateChange ( bool _enable );

         virtual void LogAsync ( const std::string & _message ) const;

         virtual ~Node ();

         static bool IsValidName ( const std::string & _name );
         static bool IsValidName ( const char * const _name, size_t _length );
      };
   }
}

#endif
