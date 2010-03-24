#include <sequanto/automation.h>

static int julemand;

void Ostemad ( short _value )
{
}

unsigned int get_julemand ( void )
{
   return julemand ;
}

static SQByte firmware[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

SQByteArray * get_firmware ( void )
{
   static SQByteArray ret;
   ret.m_start = &firmware[0];
   ret.m_end = &firmware[sizeof(firmware) / sizeof(SQByte)];
   return &ret;
}

static char configFile[] = "# Config file\n\nval1 = 2\nval2 = 42\nval3 = 0\n";

SQStringOut get_configuration_file ( void )
{
   SQStringOut out = sq_external_fixed_length_string ( &configFile, strlen(configFile) );
   return out;
}

void set_julemand ( unsigned int _newJulemand )
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