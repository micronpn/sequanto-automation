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

#ifndef SEQUANTO_QT_METHOD_2_PARAMETERS_H_
#define SEQUANTO_QT_METHOD_2_PARAMETERS_H_

#include <sequanto/propertynode.h>
#include <sequanto/methodinfo.h>
#include <QtCore>

namespace sequanto
{
   namespace automation
   {
      class QtMethod2Parameters : public Node
      {
      private:
         QObject * m_object;
         MethodInfo m_methodInfo;
         int m_methodIndex;

      public:
         QtMethod2Parameters ( const std::string & _name, QObject * _object, int _methodIndex );

         virtual const NodeInfo & Info () const;
         
         static bool Valid ( const QMetaMethod & _method );
         static SQValueType ConvertType ( QByteArray _typeByteArray );
      };
   }
}

#endif
