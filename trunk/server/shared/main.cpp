#include <QtGui>
#include <sequanto/automation.h>
#include <sequanto/tree.h>
#include <sequanto/QtWrapper.h>
#include "../cxx/src/cxx_root.h"
#include "config.h"

using namespace sequanto::automation;

int main ( int argc, char * argv[] )
{
   SQServer server;
   
   sq_init ();
   
   sq_server_init ( &server, 4321 );

   QApplication * application = new QApplication ( argc, argv );
   SQ_UNUSED_PARAMETER(application);
   
   ListNode * test = new ListNode ("test");

   sq_get_cxx_root()->AddChild ( test );

   QtWrapper::WrapApplication ( test );

   sq_server_poll ( &server );

   atexit ( sq_shutdown );
}
