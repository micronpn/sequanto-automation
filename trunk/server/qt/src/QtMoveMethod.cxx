#include <sequanto/QtMoveMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtAutomationMoveEvent.h>

using namespace sequanto::automation;

QtMoveMethod::QtMoveMethod( QWidget * _widget )
   : Node(SQ_UI_MOUSE_MOVE),
     m_widget ( _widget )
{
}

const NodeInfo & QtMoveMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_BOOLEAN );
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtMoveMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   Q_ASSERT ( _numberOfValues == 2 );
       
   int x = _inputValues[0].Value.m_integerValue;
   int y = _inputValues[1].Value.m_integerValue;

   QCoreApplication::postEvent ( m_widget, new QtAutomationMoveEvent(x, y) );

   sq_value_boolean ( &_output, SQ_TRUE );
}

QtMoveMethod::~QtMoveMethod()
{
}
