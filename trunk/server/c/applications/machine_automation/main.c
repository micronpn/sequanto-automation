#include <sequanto/automation.h>
#include <sequanto/thread.h>

#include "config.h"

#include "machine_automation.h"

#ifdef SQ_WIN32
#include <windows.h>

int server_process_id ()
{
   return GetCurrentProcessId();
}

#else

#include <unistd.h>

int server_process_id ()
{
    return getpid();
}

#endif

const char * server_version ( void )
{
   static char buff[] = SVN_REVISION;
   
   return buff;
}

void server_quit ( void )
{
   sq_server_destroy ( sq_server_get_instance() );
}

int main ( int argc, char * argv[] )
{
   static SQServer server;
   int portNumber = 4321;
   
   sq_init ();   
   
   if ( argc > 1 )
   {
      portNumber = atoi ( argv[argc - 1] );
   }
   
   sq_server_init ( &server, portNumber );
   
   sq_server_poll ( &server );

   sq_server_join ( &server );

   sq_shutdown ();
}
