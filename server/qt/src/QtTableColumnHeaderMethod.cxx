#include <sequanto/QtTableColumnHeaderMethod.h>
#include <sequanto/QtTableColumnsProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTableColumnHeaderMethod::QtTableColumnHeaderMethod()
   : Node(SQ_UI_TABLE_COLUMN_HEADER)
{
}

const NodeInfo & QtTableColumnHeaderMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_STRING );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTableColumnHeaderMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
   
   Q_ASSERT ( _numberOfValues == 1 );
       
   int column = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   Q_ASSERT ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   Q_ASSERT ( tableWidget != NULL );
   
   column = QtTableColumnsProperty::TranslateColumn(tableWidget, column);
   
   QTableWidgetItem * item = tableWidget->horizontalHeaderItem(column);
   if ( item != NULL )
   {
      sq_value_string_copy ( &_output, QtWrapper::ToString(item->text()).c_str() );
   }
   else
   {
      sq_value_string_empty ( &_output );
   }
}
