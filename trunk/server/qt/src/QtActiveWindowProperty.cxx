#include <sequanto/QtActiveWindowProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <QtGui>
#include <stdexcept>

using namespace sequanto::automation;

const std::string QtActiveWindowProperty::NO_ACTIVE_WINDOW ( "" );

QtActiveWindowProperty::QtActiveWindowProperty ()
   : ReadOnlyStringPropertyNode ( SQ_UI_ROOT_ACTIVE_WINDOW ),
     m_currentActiveWindow ( "<NULL>" )
{
}

const std::string & QtActiveWindowProperty::GetValue()
{
   QWidget * activeWindow = QApplication::activeWindow();
   
   if ( activeWindow == NULL )
   {
      m_currentActiveWindow = NO_ACTIVE_WINDOW;
   }
   else
   {
      if ( !activeWindow->isVisible() )
      {
         QtWrapper::Log ( QString("ERR: The active window is not visible, this is too weird to be true, contact Sequanto..") );
      }
      m_currentActiveWindow = QtWrapper::GetObjectName(activeWindow);
   }
   
   return m_currentActiveWindow;
}

void QtActiveWindowProperty::TrySendUpdate ()
{
   std::string previousActiveWindow ( m_currentActiveWindow );
   
   // Calling GetValue updates m_currentActiveWindow
   GetValue();
   
   if ( m_currentActiveWindow != previousActiveWindow )
   {
      PropertyNode::SendUpdate ();
      QtWrapper::ActiveWindowChanged();
   }
}

QtActiveWindowProperty::~QtActiveWindowProperty()
{
}
