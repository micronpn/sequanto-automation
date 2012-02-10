/*
 * Copyright 2010-2012 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
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

#ifndef SEQUANTO_MONITORNODE_H_
#define SEQUANTO_MONITORNODE_H_

#include <sequanto/node.h>
#include <cassert>
#include <memory>

namespace sequanto
{
   namespace automation
   {
      class SQ_DECL MonitorNode : public sequanto::automation::Node
      {
      private:
         std::auto_ptr<NodeInfo> m_nodeInfo;
         bool m_enabled;

      public:
         MonitorNode ( const std::string & _name, SQValueType _parameter1 );
         MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2 );
         MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3 );
         MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3, SQValueType _parameter4 );

         virtual const NodeInfo & Info () const;
         virtual bool IsMonitorEnabled () const;
         virtual void HandleMonitorStateChange ( bool _enable );
         virtual void SendUpdate( const SQValue * _values, size_t _numberOfValues );

         virtual ~MonitorNode ();
      };

      template<size_t NUMBER_OF_PARAMETERS>
      class SQ_DECL MonitorNodeInfo : public sequanto::automation::NodeInfo
      {
      private:
         SQValueType m_parameters[NUMBER_OF_PARAMETERS];
         
      public:
         MonitorNodeInfo ( SQValueType _parameter1 )
         {
            assert ( NUMBER_OF_PARAMETERS == 1 );

            m_parameters[0] = _parameter1;
         }

         MonitorNodeInfo ( SQValueType _parameter1, SQValueType _parameter2 )
         {
            assert ( NUMBER_OF_PARAMETERS == 2 );

            m_parameters[0] = _parameter1;
            m_parameters[1] = _parameter2;
         }

         MonitorNodeInfo ( SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3 )
         {
            assert ( NUMBER_OF_PARAMETERS == 3 );

            m_parameters[0] = _parameter1;
            m_parameters[1] = _parameter2;
            m_parameters[2] = _parameter3;
         }

         MonitorNodeInfo ( SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3, SQValueType _parameter4 )
         {
            assert ( NUMBER_OF_PARAMETERS == 4 );

            m_parameters[0] = _parameter1;
            m_parameters[1] = _parameter2;
            m_parameters[2] = _parameter3;
            m_parameters[3] = _parameter4;
         }
         
         virtual Type GetType () const
         {
            return NodeInfo::SQ_NODE_TYPE_MONITOR;
         }
         
         virtual size_t GetNumberOfParameters () const
         {
            return NUMBER_OF_PARAMETERS;
         }
         
         virtual SQValueType GetParameterType ( size_t _parameterIndex ) const
         {
            return m_parameters[_parameterIndex];
         }
      };
   }
}

#endif
