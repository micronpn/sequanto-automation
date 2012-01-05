#include <sequanto/QtTabsTabWidthMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <cassert>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTabsTabWidthMethod::QtTabsTabWidthMethod()
   : Node(SQ_UI_TABS_TAB_WIDTH)
{
}

const NodeInfo & QtTabsTabWidthMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_INTEGER );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTabsTabWidthMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_numberOfValues);
   SQ_UNUSED_PARAMETER(_inputValues);
   SQ_UNUSED_PARAMETER(_output);
   
   assert ( _numberOfValues == 1 );
       
   //int index = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTabWidget * tabWidget = qobject_cast<QTabWidget*>(widgetNode->widget());
   assert ( tabWidget != NULL );
   
   sq_value_integer ( &_output, 60 );
}
