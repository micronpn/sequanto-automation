#include <sequanto/automation.h>

static int julemand;

int get_julemand ( void )
{
   return julemand ;
}

void set_julemand ( int _newJulemand )
{
   julemand = _newJulemand;
}

int multiply ( int _parameterOne, int _parameterTwo )
{
   return _parameterOne * _parameterTwo;
}

void close ( void )
{
    //   sq_server_stop ( server );
}

const char * const firmware_version ( void )
{
   return sq_get_constant_string(SQ_STRING_CONSTANT(SVN_REVISION));
}

int main ( int argc, char * argv[] )
{
   static SQServer server;
    
   sq_init ();   
    
   sq_server_init ( &server, 4321 );

   while ( SQ_TRUE )
   {
      sq_server_poll ( &server );
   }
   
   sq_shutdown ();   
}
