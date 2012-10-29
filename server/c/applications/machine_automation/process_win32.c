#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>

#include "common.h"

#define MAX_PROCESSES (1024 * 10)

void process_refresh_internal ()
{
   DWORD processIds[MAX_PROCESSES];
   HANDLE hProcess;
   DWORD i;
   char szProcessName[MAX_PATH];
   PROCESS_MEMORY_COUNTERS processMemoryCounters;
   DWORD numberOfProcesses;

   EnumProcesses ( processIds, sizeof(processIds), &numberOfProcesses );

   process_resize_internal_buffer(numberOfProcesses);

   for ( i = 0; i < numberOfProcesses; i++ )
   {
      g_processes[i].m_id = processIds[i];

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
}
