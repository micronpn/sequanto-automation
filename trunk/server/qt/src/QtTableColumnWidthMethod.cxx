#include <sequanto/QtTableColumnWidthMethod.h>
#include <sequanto/QtTableColumnsProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <cassert>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTableColumnWidthMethod::QtTableColumnWidthMethod()
   : Node(SQ_UI_NODE_COLUMN_WIDTH)
{
}

const NodeInfo & QtTableColumnWidthMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_INTEGER );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTableColumnWidthMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
   
   assert ( _numberOfValues == 1 );
       
   int column = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   assert ( tableWidget != NULL );

   column = QtTableColumnsProperty::TranslateColumn(tableWidget, column);
   
   int columnWidth = tableWidget->columnWidth ( column );
   sq_value_integer ( &_output, columnWidth );
}
