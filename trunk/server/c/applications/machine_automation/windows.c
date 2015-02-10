#include <sequanto/automation.h>
#include <string.h>

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
   return NULL;
}

void windows_ref ( SQByteArray * _pointer )
{
}

void windows_unref ( SQByteArray * _pointer )
{
}

int windows_process_id ( SQByteArray * _pointer )
{
}

int windows_x ( SQByteArray * _pointer )
{
    return -1;
}

int windows_y ( SQByteArray * _pointer )
{
    return -1;
}

int windows_width ( SQByteArray * _pointer )
{
    return 0;
}

int windows_height ( SQByteArray * _pointer )
{
    return 0;
}

char * windows_name ( SQByteArray * _pointer )
{
    return strdup("");
}

char * windows_role ( SQByteArray * _pointer )
{
    return strdup("");
}

char * windows_text ( SQByteArray * _pointer )
{
    return strdup("");
}

int windows_actions ( SQByteArray * _pointer )
{
    return 0;
}

char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
{
    return strdup("");
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
}

long windows_children ( SQByteArray * _pointer )
{
   return 0;
}

SQByteArray * windows_child ( SQByteArray * _pointer, long _childNum )
{
   return NULL;
}

