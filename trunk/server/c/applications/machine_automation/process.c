#include <sequanto/automation.h>
#include <stdio.h>

#include "machine_automation.h"

#include "config.h"

static char UNKNOWN[] = "<UNKNOWN>";
static int g_numberOfProcesses = 0;
static struct Process
{
   int m_id;
   char * m_name;
   char * m_filename;
   int m_memoryUsage;
} *g_processes;

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
      return UNKNOWN;
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
      return UNKNOWN;
   }
   else
   {
      return g_processes[_index].m_filename;
   }
}

int process_memory ( int _index )
{
   return g_processes[_index].m_memoryUsage;
}

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>

#define MAX_PROCESSES (1024 * 10)

void process_refresh ()
{
   DWORD processIds[MAX_PROCESSES];
   HANDLE hProcess;
   DWORD i;
   char szProcessName[MAX_PATH];
   PROCESS_MEMORY_COUNTERS processMemoryCounters;

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

   EnumProcesses ( processIds, sizeof(processIds), &g_numberOfProcesses );
   g_numberOfProcesses /= sizeof(DWORD);
   g_processes = malloc ( g_numberOfProcesses * sizeof(struct Process) );
   
   for ( i = 0; i < g_numberOfProcesses; i++ )
   {
      g_processes[i].m_id = processIds[i];
      g_processes[i].m_name = NULL;
      g_processes[i].m_filename = NULL;
      g_processes[i].m_memoryUsage = 0;

      hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i] );
      
      // Get the process name.
      if (NULL != hProcess )
      {
         HMODULE hMod;
         DWORD cbNeeded;
         
         if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
                                  &cbNeeded) )
         {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               sizeof(szProcessName)/sizeof(char) );
            
            g_processes[i].m_name = _strdup ( szProcessName );

            GetModuleFileNameEx ( hProcess, hMod, szProcessName, 
                                  sizeof(szProcessName)/sizeof(char) );
            
            g_processes[i].m_filename = _strdup ( szProcessName );
         }

         if ( GetProcessMemoryInfo ( hProcess, &processMemoryCounters, sizeof(PROCESS_MEMORY_COUNTERS) ) )
         {
            g_processes[i].m_memoryUsage = processMemoryCounters.WorkingSetSize;
         }
      }
      CloseHandle ( hProcess );
   }

   sq_process_count_updated ( g_numberOfProcesses );
}

#else

#include <string.h>

void process_refresh ()
{
}

int process_exec ( const char * _commandLine )
{
    size_t lengthOfCommandLine = strlen(_commandLine);
    // "sh -c " +_commandLine
    char * commandLine = malloc ( lengthOfCommandLine + 6 );
    memcpy ( commandLine, "sh -c ", 5 );
    memcpy ( commandLine + 5, _commandLine, lengthOfCommandLine + 1 );
}

#endif
