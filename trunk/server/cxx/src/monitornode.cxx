#include <sequanto/monitornode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>

using namespace std;
using namespace sequanto::automation;

MonitorNode::MonitorNode ( const std::string & _name, SQValueType _parameter1 )
   : Node(_name),
     m_nodeInfo ( new MonitorNodeInfo<1>(_parameter1) ),
     m_enabled(false)
{
}

MonitorNode::MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2 )
   : Node(_name),
     m_nodeInfo ( new MonitorNodeInfo<2>(_parameter1, _parameter2) ),
     m_enabled(false)
{
}

MonitorNode::MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3 )
   : Node(_name),
     m_nodeInfo ( new MonitorNodeInfo<3>(_parameter1, _parameter2, _parameter3) ),
     m_enabled(false)
{
}

MonitorNode::MonitorNode ( const std::string & _name, SQValueType _parameter1, SQValueType _parameter2, SQValueType _parameter3, SQValueType _parameter4 )
   : Node(_name),
     m_nodeInfo ( new MonitorNodeInfo<4>(_parameter1, _parameter2, _parameter3, _parameter4) ),
     m_enabled(false)
{
}

const NodeInfo & MonitorNode::Info () const
{
   return *m_nodeInfo;
}

bool MonitorNode::IsMonitorEnabled () const
{
   return m_enabled;
}

void MonitorNode::HandleMonitorStateChange ( bool _enable )
{
   m_enabled = _enable;
}
 
void MonitorNode::SendUpdate( const SQValue * _values, size_t _numberOfValues )
{
   std::string fullname ( this->GetFullName() );
   SQServer * server = sq_server_get_instance();
   sq_protocol_write_update_with_values_message ( server->m_stream, fullname.c_str(), _values, _numberOfValues );
}

MonitorNode::~MonitorNode()
{
}
