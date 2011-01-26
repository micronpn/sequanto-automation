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

#ifndef SEQUANTO_LISTNODE_H_
#define SEQUANTO_LISTNODE_H_

#include <string>
#include <map>
#include <sequanto/node.h>
#include <sequanto/mutex.h>

namespace sequanto
{
   namespace automation
   {
      class ListNode : public sequanto::automation::Node
      {
      public:
         typedef std::map<std::string, Node*> NodeMap;

      private:
         NodeMap m_children;
         Mutex m_mutex;
         
      public:
         ListNode ( const std::string & _name );
         
         virtual Node * FindChild ( const std::string & _name ) const;
         virtual Iterator * ListChildren () const;
         
         virtual const NodeInfo & Info () const;
         
         void AddChild ( Node * _child );
         void RemoveChild ( std::string _name );
         bool HasChild ( std::string _name );
         
         /**
          * Send a !UPDATE message to the client to notify that this list has changed.
          */
         virtual void SendUpdate ();
         
         virtual ~ListNode ();
      };
   }
}

#endif
