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

#ifndef SEQUANTO_TREE_H_
#define SEQUANTO_TREE_H_

#include <string>
#include <sequanto/nodeinfo.h>
#include <sequanto/methodinfo.h>
#include <sequanto/node.h>
#include <sequanto/listnode.h>
#include <sequanto/propertynode.h>
#include <sequanto/constantnode.h>
#include <sequanto/readonlypropertynode.h>
#include <sequanto/readwritepropertynode.h>
#include <sequanto/stream.h>

namespace sequanto
{
   namespace automation
   {
      class Tree
      {
      private:
         Node * m_root;
         
      public:
		 Tree ();
         Node * FindNode ( const char * _path );
         SQBool HandleInfo ( SQStream * _stream, const char * _path );
         SQBool HandleList ( SQStream * _stream, const char * _path );
         SQBool HandleEnable ( SQStream * _stream, const char * _path );
         SQBool HandleDisable ( SQStream * _stream, const char * _path );
         SQBool HandleGet ( SQStream * _stream, const char * _path );
         SQBool HandleSet ( SQStream * _stream, const char * _path, const SQValue * const _value );
         SQBool HandleCall ( SQStream * _stream, const char * _path, const SQValue * const _values, int _numberOfValues );
         void SetRoot ( Node * _root );

         static void WriteOkValue ( SQStream * _stream, const SQValue & _value );
      };
   }
}

#endif
