#include <sequanto/automation.h>
#include "config.h"
#include "liab_test_automation.h"

#include <stdio.h>

#define DEV_NANOMOD "/dev/nanomod"

const char * automation_server_version ()
{
    return SVN_REVISION;
}

#define MAX_FILE_SIZE 1024
//static const size_t MAX_FILE_SIZE = 1024;
static char file_contents_buffer[MAX_FILE_SIZE];

const char * get_file_contents ( const char * _filename )
{
    FILE * fp = fopen(_filename, "r");
    fscanf ( fp, "%s", file_contents_buffer );
    fclose ( fp );
    return file_contents_buffer;
}

const char * os_version ()
{
    return get_file_contents ( "/proc/sys/kernel/version" );
}

const char * os_type ()
{
    return get_file_contents ( "/proc/sys/kernel/ostype" );
}

const char * os_release ()
{
    return get_file_contents ( "/proc/sys/kernel/osrelease" );
}

static SQBool button1;
static SQBool button2;

SQBool get_button_1 ()
{
    return button1;
}

SQBool get_button_2 ()
{
    return button2;
}

void update_buttons ( SQServer * _server )
{
   SQ_UNUSED_PARAMETER(_server);
   
    FILE * fp;
    int ret;
    SQBool newButton1, newButton2;
    
    fp = fopen ( DEV_NANOMOD, "r" );
    if ( fp != NULL )
    {
        ret = fgetc ( fp );
        newButton1 = ret == '1' ? SQ_TRUE : SQ_FALSE;
        ret = fgetc ( fp );
        newButton2 = ret == '1' ? SQ_TRUE : SQ_FALSE;
        fclose ( fp );
    }
    else
    {
        fprintf ( stderr, "Could not open nanomod device (%s) for reading.\n", DEV_NANOMOD );
        return;
    }
    
    if ( newButton1 != button1 )
    {
        button1 = newButton1;
        sq_button_1_updated ( button1 );
    }

    if ( newButton2 != button2 )
    {
        button2 = newButton2;
        sq_button_2_updated ( button2 );
    }
}

int main ()
{
   static SQServer server;
   button1 = SQ_FALSE;
   button2 = SQ_FALSE;
   
   sq_init ();   
   
   sq_server_init ( &server, 4321 );
   
   while ( SQ_TRUE )
   {
       update_buttons( &server );
       
      //printf ( "Button1: %i, Button2: %i.\n", button1, button2 );
      
      sq_server_poll ( &server );
   }
   
   sq_shutdown ();
}
