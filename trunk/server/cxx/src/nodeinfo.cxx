#include <sequanto/nodeinfo.h>

using namespace std;
using namespace sequanto::automation;


NodeInfo::NodeInfo ()
{
}

SQValueType NodeInfo::GetReturnType () const
{
   return VALUE_TYPE_NO_VALUE;
}

size_t NodeInfo::GetNumberOfParameters () const
{
   return 0;
}

SQValueType NodeInfo::GetParameterType ( size_t _parameterIndex ) const
{
   SQ_UNUSED_PARAMETER(_parameterIndex);
   return VALUE_TYPE_NO_VALUE;
}

NodeInfo::~NodeInfo()
{
}
