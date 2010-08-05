#include <sequanto/readonlypropertynode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>

using namespace std;
using namespace sequanto::automation;


ReadOnlyPropertyNode::ReadOnlyPropertyNode ( const std::string & _name )
: PropertyNode ( _name )
{
}

void ReadOnlyPropertyNode::HandleSet ( const SQValue * const _value )
{
   throw std::exception ( "Can not set value of a read-only property." );
}

ReadOnlyPropertyNode::~ReadOnlyPropertyNode ()
{
}

ReadOnlyIntegerPropertyNode::ReadOnlyIntegerPropertyNode ( const std::string & _name )
: ReadOnlyPropertyNode ( _name )
{
}

const NodeInfo & ReadOnlyIntegerPropertyNode::Info() const
{
   return GetReadOnlyIntegerNodeInfo();
}

void ReadOnlyIntegerPropertyNode::HandleGet ( SQValue & _outputValue )
{
   sq_value_integer ( &_outputValue, this->GetValue() );
}

void ReadOnlyIntegerPropertyNode::SendUpdate ()
{
   SQValue value;
   sq_value_init ( &value );
   sq_value_integer ( &value, GetValue() );
   PropertyNode::SendUpdate ( value );
   sq_value_free( &value );
}

ReadOnlyIntegerPropertyNode::~ReadOnlyIntegerPropertyNode()
{
}

ReadOnlyStringPropertyNode::ReadOnlyStringPropertyNode ( const std::string & _name )
: ReadOnlyPropertyNode ( _name )
{
}

const NodeInfo & ReadOnlyStringPropertyNode::Info() const
{
   return GetReadOnlyStringNodeInfo();
}

void ReadOnlyStringPropertyNode::HandleGet ( SQValue & _outputValue )
{
   sq_value_const_string ( &_outputValue, this->GetValue().c_str() );
}

void ReadOnlyStringPropertyNode::SendUpdate ()
{
   SQValue value;
   sq_value_init ( &value );
   sq_value_const_string ( &value, GetValue().c_str() );
   PropertyNode::SendUpdate ( value );
   sq_value_free( &value );
}

ReadOnlyStringPropertyNode::~ReadOnlyStringPropertyNode()
{
}

ReadOnlyBooleanPropertyNode::ReadOnlyBooleanPropertyNode ( const std::string & _name )
: ReadOnlyPropertyNode ( _name )
{
}

const NodeInfo & ReadOnlyBooleanPropertyNode::Info() const
{
   return GetReadOnlyBooleanNodeInfo();
}

void ReadOnlyBooleanPropertyNode::HandleGet ( SQValue & _outputValue )
{
   sq_value_boolean ( &_outputValue, this->GetValue() );
}

void ReadOnlyBooleanPropertyNode::SendUpdate ()
{
   SQValue value;
   sq_value_init ( &value );
   sq_value_boolean ( &value, GetValue() );
   PropertyNode::SendUpdate ( value );
   sq_value_free( &value );
}

ReadOnlyBooleanPropertyNode::~ReadOnlyBooleanPropertyNode()
{
}
