#include <sequanto/QtNativeIdProperty.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/ui.h>

using namespace sequanto::automation;

QtNativeIdProperty::QtNativeIdProperty ()
   : ReadOnlyPropertyNode(SQ_UI_NODE_NATIVE_ID)
{
}
   
const NodeInfo & QtNativeIdProperty::Info () const 
{
   return GetReadOnlyByteArrayNodeInfo();
}

void QtNativeIdProperty::HandleGet ( SQValue & _value )
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(this->GetParent());
   if ( widgetNode != NULL )
   {
      widgetNode->HandleGetNativeId ( _value );
      return;
   }
   sq_value_const_byte_array ( &_value, NULL, 0 );
}
