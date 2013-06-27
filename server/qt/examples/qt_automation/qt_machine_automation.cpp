#include <vector>

#include <QtGui>

#include <sequanto/automation.h>
#include <sequanto/thread.h>

#include "mainwin.h"

int main ( int _argc, char * _argv[] )
{
   QApplication app ( _argc, _argv );

   static SQServer server;
    
   sq_init ();   
    
   sq_server_init ( &server, 4321 );

   MainWin mainWin;
   mainWin.show();

   //QtWrapper::Wrap ( qt_root, &mainWin );
   
   sq_server_poll ( &server );

   atexit(sq_shutdown);

   return app.exec();
}
