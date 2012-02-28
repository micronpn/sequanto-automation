#include <sequanto/QtListLineHeightMethod.h>
#include <sequanto/QtListLinesProperty.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>

#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtListLineHeightMethod::QtListLineHeightMethod()
   : Node(SQ_UI_LIST_LINE_HEIGHT)
{
}

const NodeInfo & QtListLineHeightMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_INTEGER );
   
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtListLineHeightMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
   
   Q_ASSERT ( _numberOfValues == 1 );
       
   int line = _inputValues[0].Value.m_integerValue;
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   Q_ASSERT ( widgetNode != NULL );
   
   QListWidget * listWidget = qobject_cast<QListWidget*>(widgetNode->widget());
   Q_ASSERT ( listWidget != NULL );
   
   line = QtListLinesProperty::TranslateLine(listWidget, line);
   
   QListWidgetItem * item = listWidget->item ( line );
   if ( item == NULL )
   {
      throw std::runtime_error ("No line with that index index.");
   }
   else
   {
      QRect rect = listWidget->visualItemRect ( item );
      sq_value_integer ( &_output, rect.height() );
   }
}
