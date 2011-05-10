#include <sequanto/QtInputMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

#include <cassert>
#include <QtGui>

using namespace sequanto::automation;

QtInputMethod::QtInputMethod()
   : Node(SQ_UI_NODE_INPUT)
{
}

const NodeInfo & QtInputMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_VOID );
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_STRING );
   }
   return info;
}
   
void QtInputMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
      
   assert ( _numberOfValues == 1 );

   std::string text ( _inputValues[0].Value.m_stringValue );
       
   QtWidgetNode * parent = dynamic_cast<QtWidgetNode*> ( this->GetParent() );
   if ( parent != NULL )
   {
      QWidget * receiver = parent->widget();
           
      QApplication::postEvent ( receiver, new QKeyEvent( QEvent::KeyPress, Qt::Key_unknown, Qt::NoModifier, text.c_str(), false, text.length() ) );
      QApplication::postEvent ( receiver, new QKeyEvent( QEvent::KeyRelease, Qt::Key_unknown, Qt::NoModifier, text.c_str(), false, text.length() ) );
   }
}

QtInputMethod::~QtInputMethod()
{
}
