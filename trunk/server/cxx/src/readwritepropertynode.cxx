#include <sequanto/readwritepropertynode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>

using namespace std;
using namespace sequanto::automation;

IntegerPropertyNode::IntegerPropertyNode(const std::string &_name)
  : ReadOnlyIntegerPropertyNode(_name )
{
}

const NodeInfo & IntegerPropertyNode::Info () const
{
   return GetIntegerNodeInfo();
}

void IntegerPropertyNode::HandleSet ( const SQValue * const _value )
{
   this->SetValue ( _value->Value.m_integerValue );
}

IntegerPropertyNode::~IntegerPropertyNode()
{
}

StringPropertyNode::StringPropertyNode(const std::string &_name)
  : ReadOnlyStringPropertyNode(_name )
{
}

const NodeInfo & StringPropertyNode::Info () const
{
   return GetStringNodeInfo();
}

void StringPropertyNode::HandleSet ( const SQValue * const _value )
{
   this->SetValue ( _value->Value.m_stringValue );
}

StringPropertyNode::~StringPropertyNode()
{
}

BooleanPropertyNode::BooleanPropertyNode(const std::string &_name)
  : ReadOnlyBooleanPropertyNode(_name )
{
}

const NodeInfo & BooleanPropertyNode::Info () const
{
   return GetBooleanNodeInfo();
}

void BooleanPropertyNode::HandleSet ( const SQValue * const _value )
{
   this->SetValue ( _value->Value.m_booleanValue == SQ_TRUE );
}

BooleanPropertyNode::~BooleanPropertyNode()
{
}

FloatPropertyNode::FloatPropertyNode(const std::string &_name)
  : ReadOnlyFloatPropertyNode(_name )
{
}

const NodeInfo & FloatPropertyNode::Info () const
{
   return GetFloatNodeInfo();
}

void FloatPropertyNode::HandleSet ( const SQValue * const _value )
{
   this->SetValue ( _value->Value.m_floatValue );
}

FloatPropertyNode::~FloatPropertyNode()
{
}
