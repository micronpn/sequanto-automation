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
   DWORD bytesReturned;
   DWORD numberOfProcesses;
   
   EnumProcesses ( processIds, sizeof(processIds), &bytesReturned );
   numberOfProcesses = bytesReturned/sizeof(DWORD);
   process_resize_internal_buffer(numberOfProcesses);

   for ( i = 0; i < numberOfProcesses; i++ )
   {
      struct Process * process = process_get_process_internal(i);
      process->m_id = processIds[i];

      hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i] );
      
      // Get the process name.
      if (NULL != hProcess )
      {
         if ( GetModuleBaseName( hProcess, NULL, szProcessName, 
                                 sizeof(szProcessName)/sizeof(char) )  )
         {
              process->m_name = _strdup ( szProcessName );
         }
         if ( GetModuleFileNameEx ( hProcess, NULL, szProcessName, 
                                  sizeof(szProcessName)/sizeof(char) ) )
         {
            process->m_filename = _strdup ( szProcessName );
         }

         if ( GetProcessMemoryInfo ( hProcess, &processMemoryCounters, sizeof(PROCESS_MEMORY_COUNTERS) ) )
         {
            process->m_memoryUsage = processMemoryCounters.WorkingSetSize;
         }
      }
      CloseHandle ( hProcess );
   }
}
