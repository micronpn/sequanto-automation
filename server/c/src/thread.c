/*
 * Copyright 2010 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "sequanto/thread.h"
#include "config.h"

#ifdef SQ_USE_THREADS

SQBool sq_thread_is_supported ( void )
{
   return SQ_TRUE;
}

typedef struct SQThreadInternalData
{
   SQThread * m_thread;
   SQThreadRunFunction m_runFunction;
   void * m_data;
} SQThreadInternalData;

void* sq_thread_internal_run_function ( void * _internalData )
{
   struct SQThreadInternalData * internalData = (SQThreadInternalData*)  _internalData;
   SQThread * thread = internalData->m_thread;
   SQThreadRunFunction runFunction = internalData->m_runFunction;
   void * data = internalData->m_data;
   free ( internalData );
   
   runFunction ( thread, data );

   return NULL;
}

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct _SQThread
{
   HANDLE m_thread;
   DWORD m_threadId;
} _SQThread;

SQThread * sq_thread_create ( SQThreadRunFunction _function, void * _data )
{
   SQThread * thread = malloc(sizeof(SQThread));
   SQThreadInternalData * internalData = malloc(sizeof(SQThreadInternalData));
   internalData->m_thread = thread;
   internalData->m_runFunction = _function;
   internalData->m_data = _data;
   thread->m_thread = CreateThread ( NULL, 0, (LPTHREAD_START_ROUTINE) sq_thread_internal_run_function, internalData, CREATE_SUSPENDED, &thread->m_threadId );
   return thread;
}

void sq_thread_start ( SQThread * _thread )
{
   ResumeThread ( _thread->m_thread );
}

void sq_thread_destroy ( SQThread * _thread )
{
   CloseHandle ( _thread->m_thread );
   //WaitForSingleObject( _thread->m_thread );
   free ( _thread );
}

void sq_thread_sleep ( int _milliseconds )
{
   Sleep ( _milliseconds );
}

#else

#include <pthread.h>

typedef struct _SQThread
{
   pthread_t m_thread;
} _SQThread;

SQThread * sq_thread_create ( SQThreadRunFunction _function, void * _data )
{
   SQThread * thread = malloc(sizeof(SQThread));
   SQThreadInternalData * internalData = malloc(sizeof(SQThreadInternalData));
   internalData->m_thread = thread;
   internalData->m_runFunction = _function;
   internalData->m_data = _data;
   pthread_create ( &thread->m_thread, NULL, sq_thread_internal_run_function, internalData );
   return thread;
}

void sq_thread_start ( SQThread * _thread )
{
}

void sq_thread_sleep ( int _milliseconds )
{
   sleep ( _milliseconds );
}

#endif

#else

typedef struct _SQThread
{
   unsigned char unused_parameter;
} _SQThread;

SQBool sq_thread_is_supported ()
{
   return SQ_FALSE;
}

SQThread * sq_thread_create ( SQThreadRunFunction _function, void * _data )
{
   return NULL;
}

void sq_thread_start ( SQThread * _thread )
{
}

void sq_thread_sleep ( int _milliseconds )
{
}

#endif
