#include <sequanto/QtUiTypeProperty.h>

using namespace sequanto::automation;

QtUiTypeProperty::QtUiTypeProperty ( SQWidgetType _type )
   : ReadOnlyPropertyNode(SQ_UI_WIDGET_TYPE),
     m_type(_type)
{
}

const NodeInfo & QtUiTypeProperty::Info() const
{
   return GetReadOnlyStringNodeInfo();
}

void QtUiTypeProperty::HandleGet ( SQValue & _outputValue )
{
   sq_value_const_string ( &_outputValue, sq_widget_type_to_string(m_type) );
}
