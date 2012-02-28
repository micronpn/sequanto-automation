#include <sequanto/QtTableCellTextMethod.h>
#include <sequanto/QtTableRowsProperty.h>
#include <sequanto/QtTableColumnsProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

//#include <sequanto/QtAutomationTableCellTextEvent.h>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtTableCellTextMethod::QtTableCellTextMethod()
   : Node(SQ_UI_TABLE_CELL_TEXT)
{
}

const NodeInfo & QtTableCellTextMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_STRING );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtTableCellTextMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
   
   Q_ASSERT ( _numberOfValues == 2 );
       
   int row = _inputValues[0].Value.m_integerValue;
   int column = _inputValues[1].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   Q_ASSERT ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   Q_ASSERT ( tableWidget != NULL );
   
   row = QtTableRowsProperty::TranslateRow ( tableWidget, row );
   column = QtTableColumnsProperty::TranslateColumn ( tableWidget, column );
   
   QTableWidgetItem * item = tableWidget->item ( row, column );
   if ( item == NULL )
   {
      throw std::runtime_error ("No cell at that row,column index.");
   }
   else
   {
      std::string text ( QtWrapper::ToString ( item->text() ) );
      sq_value_string_copy ( &_output, text.c_str() );
   }
   
   /*
   QtAutomationTableCellTextEvent * event = new QtAutomationTableCellTextEvent(x, y);
   if ( !QtAutomationTableCellTextEvent::wait ( event, window, TIMEOUT ) )
   {
      throw std::runtime_error ("Mouse move event was never handled.");
   }
   */
}
