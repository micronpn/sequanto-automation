#include <vector>

#include <QtGui>

#include <sequanto/automation.h>
#include <sequanto/thread.h>

#include "mainwin.h"

extern "C" void windows_init_if_not_already();

int main ( int _argc, char * _argv[] )
{
   QApplication app ( _argc, _argv );

   static SQServer server;
    
   sq_init ();   
    
   sq_server_init ( &server, 4321 );

   windows_init_if_not_already();

   MainWin mainWin;
   mainWin.show();

   //QtWrapper::Wrap ( qt_root, &mainWin );
   
   sq_server_poll ( &server );

   atexit(sq_shutdown);

   return app.exec();
}
