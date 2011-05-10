#include <sequanto/QtActiveWindowProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <QtGui>

using namespace sequanto::automation;

QtActiveWindowProperty::QtActiveWindowProperty ()
   : PropertyNode ( SQ_UI_NODE_ACTIVE_WINDOW ),
     m_previousActiveWindow ( "<NULL>" )
{
}

void QtActiveWindowProperty::InternalGet ( SQValue * _outputValue, bool _sendUpdateIfNeeded )
{
   static std::string NO_ACTIVE_WINDOW ( "" );

   std::string newActiveWindow;
   QWidget * activeWindow = QApplication::activeWindow();
   if ( activeWindow == NULL )
   {
      newActiveWindow = NO_ACTIVE_WINDOW;
   }
   else
   {
      newActiveWindow = QtWrapper::GetObjectName(activeWindow);
   }

   if ( _outputValue != NULL )
   {
      sq_value_string_copy ( _outputValue, newActiveWindow.c_str() );
   }

   if ( newActiveWindow != m_previousActiveWindow )
   {
      m_previousActiveWindow = newActiveWindow;

      if ( _sendUpdateIfNeeded )
      {
         // Re-use _outputValue
         if( _outputValue != NULL )
         {
            PropertyNode::SendUpdate ( *_outputValue );
         }
         else
         {
            SQValue value;
            sq_value_init ( &value );
            sq_value_const_string ( &value, m_previousActiveWindow.c_str() );
            PropertyNode::SendUpdate ( value );
            sq_value_free ( &value );
         }
      }
   }
}

const NodeInfo & QtActiveWindowProperty::Info () const
{
   return this->GetReadOnlyStringNodeInfo();
}

void QtActiveWindowProperty::HandleGet ( SQValue & _outputValue )
{
   InternalGet ( &_outputValue, false );
}

void QtActiveWindowProperty::HandleSet ( const SQValue * const _value )
{
   SQ_UNUSED_PARAMETER(_value);
   
   throw std::runtime_error ( "Can not set the active window" );
}

void QtActiveWindowProperty::TrySendUpdate ()
{
   InternalGet ( NULL, true );
}

QtActiveWindowProperty::~QtActiveWindowProperty()
{
}
