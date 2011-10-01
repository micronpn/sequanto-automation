#include <sequanto/QtMouseClickMethod.h>
#include <sequanto/QtAutomationMouseClickEvent.h>
#include <sequanto/methodinfo.h>
#include <sequanto/ui.h>
#include <QtGui>
#include <cassert>
#include <stdexcept>

using namespace sequanto::automation;

QtMouseClickMethod::QtMouseClickMethod()
   : Node(SQ_UI_MOUSE_CLICK)
{
}

const NodeInfo & QtMouseClickMethod::Info () const
{
   static MethodInfo info ( VALUE_TYPE_VOID );
   if ( info.GetNumberOfParameters() == 0 )
   {
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
      info.AddParameter ( VALUE_TYPE_INTEGER );
   }
   return info;
}
   
void QtMouseClickMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_output);
      
   assert ( _numberOfValues == 3 );
       
   int x = _inputValues[0].Value.m_integerValue;
   int y = _inputValues[1].Value.m_integerValue;
   int mouseButton = _inputValues[2].Value.m_integerValue;
      
   Qt::MouseButton button = Qt::NoButton;
   switch ( mouseButton )
   {
   case 0:
      button = Qt::LeftButton;
      break;

   case 1:
      button = Qt::RightButton;
      break;

   case 2:
      button = Qt::MidButton;
      break;
   }
   
   QWidget * window = QApplication::activeWindow();
   if ( window != NULL )
   {
      QtAutomationMouseClickEvent * event = new QtAutomationMouseClickEvent(x, y, button);
      if ( !QtAutomationMouseClickEvent::wait ( event, window, TIMEOUT ) )
      {
         throw std::runtime_error ("Mouse click event was never handled.");
      }
   }
}

QtMouseClickMethod::~QtMouseClickMethod()
{
}
