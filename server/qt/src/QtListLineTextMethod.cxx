#include <sequanto/QtListLineTextMethod.h>
#include <sequanto/QtListLinesProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtListLineTextMethod::QtListLineTextMethod()
   : Node(SQ_UI_LIST_LINE_TEXT)
{
}

const NodeInfo & QtListLineTextMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_STRING );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtListLineTextMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   Q_ASSERT ( _numberOfValues == 1 );
       
   int line = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   Q_ASSERT ( widgetNode != NULL );
   
   QListWidget * listWidget = qobject_cast<QListWidget*>(widgetNode->widget());
   Q_ASSERT ( listWidget != NULL );
   
   line = QtListLinesProperty::TranslateLine ( listWidget, line );
   
   QListWidgetItem * item = listWidget->item ( line );
   if ( item == NULL )
   {
      throw std::runtime_error ("No line with that index index.");
   }
   else
   {
      std::string text ( QtWrapper::ToString ( item->text() ) );
      sq_value_string_copy ( &_output, text.c_str() );
   }
}
