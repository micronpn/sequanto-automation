#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include "common.h"

static int process_is_int ( const struct dirent * _entry )
{
    const char * c;

    if ( _entry->d_type == DT_DIR )
    {
        for ( c = _entry->d_name; *c != '\0'; c++ )
        {
            if ( !isdigit(*c) )
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void process_refresh_internal ()
{
    struct dirent ** names = NULL;
    size_t i = 0;
    int num = scandir ( "/proc", &names, &process_is_int, NULL );
    char exeLink[100];
    char exe[1024];
    char cmdLineFile[100];
    char cmdLine[1024];
    size_t cmdLineLength;
    size_t j;
    FILE * fp;
    ssize_t len;
    struct stat statBuf;
    struct Process * process;

    if ( num == -1 )
    {
        process_resize_internal_buffer ( 0 );
    }
    else
    {
        process_resize_internal_buffer ( num );
        for ( i = 0; i < ((size_t) num); i++ )
        {
            process = process_get_process_internal ( i );
            process->m_id = atoi(names[i]->d_name);
            sprintf ( exeLink, "/proc/%i/exe", process->m_id );
            len = readlink ( exeLink, exe,1024 );
            if ( len != -1 )
            {
                exe[len] = '\0';
                process->m_filename = strdup(exe);
            }

            sprintf ( exeLink, "/proc/%i", process->m_id );
            stat ( exeLink, &statBuf );
            process->m_owner = statBuf.st_uid;

            sprintf ( cmdLineFile, "/proc/%i/cmdline", process->m_id );
            fp = fopen ( cmdLineFile, "rb" );
            cmdLineLength = fread ( &cmdLine, 1, 1024, fp );
            fclose ( fp );

            for ( j = 0; j < cmdLineLength; j++ )
            {
               if ( cmdLine[j] == '\0' )
               {
                   cmdLine[j] = ' ';
               }
            }
            cmdLine[cmdLineLength] = '\0';
            process->m_cmdline = strdup ( cmdLine );
        }
    }
    free ( names );
}

int process_exec ( const char * _commandLine )
{
    size_t lengthOfCommandLine = strlen(_commandLine);
     /* "sh -c " +_commandLine */
    char * commandLine = malloc ( lengthOfCommandLine + 6 );
    memcpy ( commandLine, "sh -c ", 5 );
    memcpy ( commandLine + 5, _commandLine, lengthOfCommandLine + 1 );

    return -1;
}
