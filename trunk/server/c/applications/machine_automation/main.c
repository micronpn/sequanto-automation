#include <sequanto/automation.h>
#include <sequanto/thread.h>

#include "config.h"
#include "machine_automation.h"
#include "common.h"

int main ( int argc, char * argv[] )
{
   static SQServer server;
   int portNumber = 4321;

   sq_init ();

   if ( argc > 1 )
   {
      portNumber = atoi ( argv[argc - 1] );
   }

   windows_init_subsystem ();

   sq_server_init ( &server, portNumber );

   sq_server_poll ( &server );

   sq_server_join ( &server );

   windows_deinit_subsystem ();

   sq_shutdown ();

   return 0;
}
