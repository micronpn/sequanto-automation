#include <sequanto/QtMouseMoveMethod.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <sequanto/QtAutomationMouseMoveEvent.h>
#include <sequanto/QtAutomationStealFocusEvent.h>
#include <cassert>
#include <stdexcept>
#include <QtGui>

using namespace sequanto::automation;

QtMouseMoveMethod::QtMouseMoveMethod()
   : Node(SQ_UI_MOUSE_MOVE)
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
   if ( window == NULL || !window->hasFocus() )
   {
      QtAutomationStealFocusEvent * event = new QtAutomationStealFocusEvent();
      QtAutomationStealFocusEvent::wait ( event, QApplication::instance(), TIMEOUT );

      window = QApplication::activeWindow();
   }
   
   if ( window != NULL )
   {
      QtAutomationMouseMoveEvent * event = new QtAutomationMouseMoveEvent(x, y);
      if ( !QtAutomationMouseMoveEvent::wait ( event, window, TIMEOUT ) )
      {
         throw std::runtime_error ("Mouse move event was never handled.");
      }
   }
}

QtMouseMoveMethod::~QtMouseMoveMethod()
{
}
