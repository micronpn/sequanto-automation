#include <sequanto/QtMouseMoveMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtAutomationMouseMoveEvent.h>
#include <cassert>
#include <QtGui>

using namespace sequanto::automation;

QtMouseMoveMethod::QtMouseMoveMethod()
   : Node(SQ_UI_NODE_MOVE)
{
}

const NodeInfo & QtMouseMoveMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_VOID );

   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}
   
void QtMouseMoveMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
      
   assert ( _numberOfValues == 2 );
       
   int x = _inputValues[0].Value.m_integerValue;
   int y = _inputValues[1].Value.m_integerValue;

   QWidget * window = QApplication::activeWindow();
   if ( window != NULL )
   {
      QApplication::postEvent ( window, new QtAutomationMouseMoveEvent(x, y) );
   }
}

QtMouseMoveMethod::~QtMouseMoveMethod()
{
}
