#include <sequanto/propertynode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>

using namespace std;
using namespace sequanto::automation;

class PropertyNodeInfo : public NodeInfo
{
public:
   SQValueType m_valueType;
   
   PropertyNodeInfo ( SQValueType _valueType )
      : NodeInfo()
   {
      m_valueType = _valueType;
   }
   
   Type GetType () const
   {
      return NodeInfo::SQ_NODE_TYPE_READ_WRITE_PROPERTY;
   }

   SQValueType GetReturnType() const
   {
      return m_valueType;
   }
};

class ReadOnlyPropertyNodeInfo : public NodeInfo
{
public:
   SQValueType m_valueType;
   
   ReadOnlyPropertyNodeInfo ( SQValueType _valueType )
      : NodeInfo()
   {
      m_valueType = _valueType;
   }
   
   Type GetType () const
   {
      return NodeInfo::SQ_NODE_TYPE_READABLE_PROPERTY;
   }

   SQValueType GetReturnType() const
   {
      return m_valueType;
   }
};

const NodeInfo & PropertyNode::GetIntegerNodeInfo ()
{
   static PropertyNodeInfo nodeInfo ( VALUE_TYPE_INTEGER );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetFloatNodeInfo ()
{
   static PropertyNodeInfo nodeInfo ( VALUE_TYPE_FLOAT );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetStringNodeInfo ()
{
   static PropertyNodeInfo nodeInfo ( VALUE_TYPE_STRING );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetBooleanNodeInfo ()
{
   static PropertyNodeInfo nodeInfo ( VALUE_TYPE_BOOLEAN );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetByteArrayNodeInfo ()
{
   static PropertyNodeInfo nodeInfo ( VALUE_TYPE_BYTE_ARRAY );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetReadOnlyIntegerNodeInfo ()
{
   static ReadOnlyPropertyNodeInfo nodeInfo ( VALUE_TYPE_INTEGER );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetReadOnlyFloatNodeInfo ()
{
   static ReadOnlyPropertyNodeInfo nodeInfo ( VALUE_TYPE_FLOAT );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetReadOnlyStringNodeInfo ()
{
   static ReadOnlyPropertyNodeInfo nodeInfo ( VALUE_TYPE_STRING );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetReadOnlyBooleanNodeInfo ()
{
   static ReadOnlyPropertyNodeInfo nodeInfo ( VALUE_TYPE_BOOLEAN );
   return nodeInfo;
}

const NodeInfo & PropertyNode::GetReadOnlyByteArrayNodeInfo ()
{
   static ReadOnlyPropertyNodeInfo nodeInfo ( VALUE_TYPE_BYTE_ARRAY );
   return nodeInfo;
}

PropertyNode::PropertyNode ( const std::string & _name )
   : Node(_name)
{
}

void PropertyNode::SendUpdate( const SQValue & _value )
{
   std::string fullname ( this->GetFullName() );
   SQServer * server = sq_server_get_instance();
   sq_protocol_write_update_with_value_message ( server->m_stream, fullname.c_str(), &_value );
}

void PropertyNode::SendUpdate ()
{
	SQValue value;
	sq_value_init ( &value );
	HandleGet ( value );
	SendUpdate ( value );
	sq_value_free ( &value);
}

PropertyNode::~PropertyNode()
{
}
