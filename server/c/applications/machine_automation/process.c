#include <sequanto/automation.h>
#include <stdio.h>

#include "machine_automation.h"
#include "config.h"
#include "common.h"

static char EMPTY[] = "";
static int g_numberOfProcesses = 0;
struct Process * g_processes = NULL;

void process_resize_internal_buffer ( size_t _newSize )
{
   size_t i;
   
   for ( i = 0; i < g_numberOfProcesses; i++ )
   {
      if ( g_processes[i].m_name != NULL )
      {
         free ( g_processes[i].m_name );
      }
      if ( g_processes[i].m_filename != NULL )
      {
         free ( g_processes[i].m_filename );
      }
   }
   free ( g_processes );
   
   g_numberOfProcesses = _newSize;
   g_processes = (struct Process *) malloc ( g_numberOfProcesses * sizeof(struct Process) );

   for ( i = 0; i < g_numberOfProcesses; i++ )
   {
       g_processes[i].m_name = NULL;
       g_processes[i].m_filename = NULL;
       g_processes[i].m_owner = -1;
       g_processes[i].m_memoryUsage = 0;
   }

   sq_process_count_updated ( g_numberOfProcesses );
}

int process_count ()
{
   return g_numberOfProcesses;
}

int process_id ( int _index )
{
   return g_processes[_index].m_id;
}

const char * process_name ( int _index )
{
   if ( g_processes[_index].m_name == NULL )
   {
      return EMPTY;
   }
   else
   {
      return g_processes[_index].m_name;
   }
}

const char * process_filename ( int _index )
{
   if ( g_processes[_index].m_filename == NULL )
   {
      return EMPTY;
   }
   else
   {
      return g_processes[_index].m_filename;
   }
}

int process_owner ( int _index )
{
    return g_processes[_index].m_owner;
}

const char * process_cmdline ( int _index )
{
   if ( g_processes[_index].m_cmdline == NULL )
   {
      return EMPTY;
   }
   else
   {
      return g_processes[_index].m_cmdline;
   }
}

int process_memory ( int _index )
{
   return g_processes[_index].m_memoryUsage;
}

void process_refresh ()
{
   process_refresh_internal();
}
