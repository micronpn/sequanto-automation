#include <sequanto/automation.h>

void windows_init_subsystem ()
{
}

void windows_deinit_subsystem ()
{
}

int windows_desktops ( void )
{
    return 0;
}

SQByteArray * windows_desktop ( int _desktop )
{
   return sq_byte_array_create_prealloc ( 0 );
}

void windows_ref ( SQByteArray * _pointer )
{
}

void windows_unref ( SQByteArray * _pointer )
{
}

static char EMPTY[] = "";

char * windows_name ( SQByteArray * _pointer )
{
   return EMPTY;
}

long windows_children ( SQByteArray * _pointer )
{
   return 0;
}

SQByteArray * windows_child ( SQByteArray * _pointer, long _childNum )
{
   return sq_byte_array_create_prealloc ( 0 );
}
