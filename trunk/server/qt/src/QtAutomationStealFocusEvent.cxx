#include <sequanto/QtAutomationStealFocusEvent.h>
#include <sequanto/log.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace sequanto::automation;

const int QtAutomationStealFocusEvent::ID = QEvent::registerEventType();

QtAutomationStealFocusEvent::QtAutomationStealFocusEvent ()
   : QtAutomationDelayedDoneEvent( (QEvent::Type) ID)
{
}

QtAutomationStealFocusEvent * QtAutomationStealFocusEvent::clone () const
{
   return new QtAutomationStealFocusEvent ();
}

#ifdef _WIN32

static DWORD g_processId = 0;

BOOL CALLBACK GetMainWindowIdProc ( HWND _id, LPARAM _ret )
{
   DWORD processId= 0;
   GetWindowThreadProcessId ( _id, &processId );
   if ( processId == g_processId )
   {
      *((HWND *) _ret) = _id;
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}

HWND GetMainWindowId ()
{
   HWND ret = 0;

   if ( g_processId == 0 )
   {
      g_processId = GetCurrentProcessId();
   }
   EnumWindows ( &GetMainWindowIdProc, (LPARAM) &ret );
   return ret;
}

void QtAutomationStealFocusEvent::stealFocus()
{
   HWND mainWindow = GetMainWindowId ();
   sq_logf ( "Stealing focus of %x", mainWindow );
   ShowWindow ( mainWindow, SW_SHOW );
   SetForegroundWindow ( mainWindow );
}

#else

void QtAutomationStealFocusEvent::stealFocus()
{
}

#endif

QtAutomationStealFocusEvent::~QtAutomationStealFocusEvent()
{
}
