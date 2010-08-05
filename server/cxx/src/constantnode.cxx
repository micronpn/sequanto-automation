#include <sequanto/constantnode.h>

using namespace sequanto::automation;

ConstantIntegerNode::ConstantIntegerNode ( const std::string & _name, int _value )
   : ReadOnlyIntegerPropertyNode ( _name ),
     m_value ( _value )
{
}

int ConstantIntegerNode::GetValue ()
{
   return m_value;
}

ConstantIntegerNode::~ConstantIntegerNode ()
{
}


ConstantStringNode::ConstantStringNode ( const std::string & _name, const char * _value )
   : ReadOnlyStringPropertyNode ( _name ),
     m_value ( _value )
{
}

ConstantStringNode::ConstantStringNode ( const std::string & _name, const std::string & _value )
   : ReadOnlyStringPropertyNode ( _name ),
     m_value ( _value )
{
}

const std::string & ConstantStringNode::GetValue ()
{
   return m_value;
}

ConstantStringNode::~ConstantStringNode ()
{
}

ConstantBooleanNode::ConstantBooleanNode ( const std::string & _name, bool _value )
   : ReadOnlyBooleanPropertyNode ( _name ),
     m_value ( _value )
{
}

bool ConstantBooleanNode::GetValue ()
{
   return m_value;
}

ConstantBooleanNode::~ConstantBooleanNode ()
{
}
