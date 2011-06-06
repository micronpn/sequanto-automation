#include <sequanto/QtTableRowHeightMethod.h>
#include <sequanto/QtTableRowsProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <cassert>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTableRowHeightMethod::QtTableRowHeightMethod()
   : Node(SQ_UI_NODE_ROW_HEIGHT)
{
}

const NodeInfo & QtTableRowHeightMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_INTEGER );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTableRowHeightMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
   
   assert ( _numberOfValues == 1 );
       
   int row = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   assert ( tableWidget != NULL );
   
   row = QtTableRowsProperty::TranslateRow(tableWidget, row);
   
   int rowHeight = tableWidget->rowHeight ( row );
   sq_value_integer ( &_output, rowHeight );
}
