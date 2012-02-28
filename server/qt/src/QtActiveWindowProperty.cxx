#include <sequanto/QtActiveWindowProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/QtAutomationRefreshWindowsEvent.h>
#include <sequanto/lock.h>
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
   QVariant activeWindow = QtWrapper::GetPropertyValue ( QApplication::instance(), QtWrapper::active_window() );
   
   {
       Lock lock ( m_lock );
       
       m_currentActiveWindow = QtWrapper::ToString ( activeWindow.toString() );
       return m_currentActiveWindow;
   }   
}

void QtActiveWindowProperty::TrySendUpdate ()
{
    std::string previousActiveWindow;
    
    {
        Lock lock ( m_lock );

        previousActiveWindow = ( m_currentActiveWindow );
    }
    
   // Calling GetValue updates m_currentActiveWindow
   GetValue();
   
   {
       Lock lock ( m_lock );
       
       if ( m_currentActiveWindow != previousActiveWindow )
       {
           PropertyNode::SendUpdate ();
           QtWrapper::ActiveWindowChanged();
       }
   }
}

QtActiveWindowProperty::~QtActiveWindowProperty()
{
}
