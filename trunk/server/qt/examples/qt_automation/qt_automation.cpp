#include <vector>

#include <QtGui>

#include <sequanto/automation.h>
#include <sequanto/thread.h>
#include <sequanto/tree.h>
#include <sequanto/qtwrapper.h>

#include "qt_automation_automation.h"

#include "mainwin.h"

using namespace std;
using namespace sequanto::automation;

static Tree ui_tree;
static Tree qt_tree;

extern "C"
{
   SQBool sq_handle_branch_info_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleInfo ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_list_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleList ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_get_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleGet ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_set_ui ( SQStream * _stream, const char * _objectPath, const SQValue * const _value )
   {
      return ui_tree.HandleSet ( _stream, _objectPath, _value );
   }

   SQBool sq_handle_branch_disable_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleDisable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_enable_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleEnable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_call_ui ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues )
   {
      return ui_tree.HandleCall ( _stream, _objectPath, _values, _numberOfValues );
   }

   SQBool sq_handle_branch_info_qt ( SQStream * _stream, const char * _objectPath )
   {
      return qt_tree.HandleInfo ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_list_qt ( SQStream * _stream, const char * _objectPath )
   {
      return qt_tree.HandleList ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_get_qt ( SQStream * _stream, const char * _objectPath )
   {
      return qt_tree.HandleGet ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_set_qt ( SQStream * _stream, const char * _objectPath, const SQValue * const _value )
   {
      return qt_tree.HandleSet ( _stream, _objectPath, _value );
   }

   SQBool sq_handle_branch_disable_qt ( SQStream * _stream, const char * _objectPath )
   {
      return qt_tree.HandleDisable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_enable_qt ( SQStream * _stream, const char * _objectPath )
   {
      return qt_tree.HandleEnable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_call_qt ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues )
   {
      return qt_tree.HandleCall ( _stream, _objectPath, _values, _numberOfValues );
   }
}

int main ( int _argc, char * _argv[] )
{
   QApplication app ( _argc, _argv );

   static SQServer server;
    
   sq_init ();   
    
   sq_server_init ( &server, 4321 );

   ListNode ui_root ( "ui" );
   ui_tree.SetRoot ( &ui_root );

   ListNode qt_root ( "qt" );
   qt_tree.SetRoot ( &qt_root );
   
   MainWin mainWin;
   mainWin.show();

   QtWrapper::WrapUi ( &ui_root, &mainWin );
   QtWrapper::Wrap ( &qt_root, &mainWin );
   
   //mainWin.findChild<QPushButton*>("exitButton")->connect ( );

   sq_server_poll ( &server );

   atexit(sq_shutdown);

   return app.exec();
}
