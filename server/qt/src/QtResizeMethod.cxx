#include <sequanto/QtResizeMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtAutomationResizeEvent.h>

using namespace sequanto::automation;

QtResizeMethod::QtResizeMethod( QWidget * _widget )
   : Node("resize"),
     m_widget ( _widget )
{
}

const NodeInfo & QtResizeMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_BOOLEAN );
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}

void QtResizeMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   Q_ASSERT ( _numberOfValues == 2 );
       
   int width = _inputValues[0].Value.m_integerValue;
   int height = _inputValues[1].Value.m_integerValue;

   QCoreApplication::postEvent ( m_widget, new QtAutomationResizeEvent(width, height) );

   sq_value_boolean ( &_output, SQ_TRUE );
}

QtResizeMethod::~QtResizeMethod()
{
}
