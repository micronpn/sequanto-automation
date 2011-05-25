#include <sequanto/readonlypropertynode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>
#include <stdexcept>

using namespace std;
using namespace sequanto::automation;


ReadOnlyPropertyNode::ReadOnlyPropertyNode ( const std::string & _name )
: PropertyNode ( _name )
{
}

void ReadOnlyPropertyNode::HandleSet ( const SQValue * const _value )
{
   SQ_UNUSED_PARAMETER(_value);
   throw std::runtime_error ( "Can not set value of a read-only property." );
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

ReadOnlyBooleanPropertyNode::~ReadOnlyBooleanPropertyNode()
{
}


ReadOnlyFloatPropertyNode::ReadOnlyFloatPropertyNode ( const std::string & _name )
: ReadOnlyPropertyNode ( _name )
{
}

const NodeInfo & ReadOnlyFloatPropertyNode::Info() const
{
   return GetReadOnlyFloatNodeInfo();
}

void ReadOnlyFloatPropertyNode::HandleGet ( SQValue & _outputValue )
{
   sq_value_float ( &_outputValue, this->GetValue() );
}

ReadOnlyFloatPropertyNode::~ReadOnlyFloatPropertyNode()
{
}
