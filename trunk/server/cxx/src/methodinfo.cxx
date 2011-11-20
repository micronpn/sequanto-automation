#include <sequanto/methodinfo.h>

using namespace std;
using namespace sequanto::automation;


MethodInfo::MethodInfo ()
   : NodeInfo(),
     m_returnType ( VALUE_TYPE_VOID )
{
}

MethodInfo::MethodInfo ( SQValueType _returnType )
   : NodeInfo(),
     m_returnType ( _returnType )
{
}

NodeInfo::Type MethodInfo::GetType () const
{
   return NodeInfo::SQ_NODE_TYPE_CALLABLE;
}

SQValueType MethodInfo::GetReturnType () const
{
   return m_returnType;
}

size_t MethodInfo::GetNumberOfParameters () const
{
   return m_parameters.size();
}

SQValueType MethodInfo::GetParameterType ( size_t _parameterIndex ) const
{
   return m_parameters[_parameterIndex];
}

void MethodInfo::SetReturnType ( SQValueType _type )
{
   m_returnType = _type;
}

void MethodInfo::AddParameter ( SQValueType _type )
{
   m_parameters.push_back ( _type );
}

MethodInfo::~MethodInfo()
{
}

const MethodInfo & MethodInfo::voidVoid ()
{
   static MethodInfo ret;
   
   return ret;
}
