#include <sequanto/QtTabsTabNameMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <cassert>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTabsTabNameMethod::QtTabsTabNameMethod()
   : Node(SQ_UI_TABS_TAB_NAME)
{
}

const NodeInfo & QtTabsTabNameMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_STRING );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTabsTabNameMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   assert ( _numberOfValues == 1 );
       
   int index = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTabWidget * tabWidget = qobject_cast<QTabWidget*>(widgetNode->widget());
   assert ( tabWidget != NULL );
   
   std::string text ( QtWrapper::ToString ( tabWidget->tabText ( index ) ) );
   sq_value_string_copy ( &_output, text.c_str() );
}
