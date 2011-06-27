#include <sequanto/automation.h>
#include <stdio.h>

#include "config.h"

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

size_t memory_total_physical ()
{
   MEMORYSTATUS status;
   GlobalMemoryStatus ( &status );
   return status.dwTotalPhys;
}

size_t memory_available_physical ()
{
   MEMORYSTATUS status;
   GlobalMemoryStatus ( &status );
   return status.dwAvailPhys;
}

size_t memory_total_virtual ()
{
   MEMORYSTATUS status;
   GlobalMemoryStatus ( &status );
   return status.dwTotalVirtual;
}

size_t memory_available_virtual ()
{
   MEMORYSTATUS status;
   GlobalMemoryStatus ( &status );
   return status.dwAvailVirtual;
}

#else

#endif
