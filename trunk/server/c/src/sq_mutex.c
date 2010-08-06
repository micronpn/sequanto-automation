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

#ifdef SQ_USE_MUTEXES

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct _SQMutex
{
    CRITICAL_SECTION m_criticalSection;
} _SQMutex;

SQMutex * sq_mutex_create ( void )
{
    SQMutex * mutex = malloc(sizeof(SQMutex));
    InitializeCriticalSection ( &mutex->m_criticalSection );
    return mutex;
}

void sq_mutex_enter ( SQMutex * _mutex )
{
   EnterCriticalSection ( &_mutex->m_criticalSection );
}

void sq_mutex_leave ( SQMutex * _mutex )
{
    LeaveCriticalSection ( &_mutex->m_criticalSection );
}

void sq_mutex_destroy ( SQMutex * _mutex )
{
    DeleteCriticalSection ( &_mutex->m_criticalSection );
    free ( _mutex );
}

#else

#include <pthread.h>

typedef struct _SQMutex
{
    pthread_mutex_t m_mutex;
} _SQMutex;


SQMutex * sq_mutex_create ()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init ( &attr );
    pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE_NP );
    
    SQMutex * mutex = malloc(sizeof(SQMutex));
    pthread_mutex_init ( &mutex->m_mutex, &attr );

    pthread_mutexattr_destroy ( &attr );
    return mutex;
}

void sq_mutex_enter ( SQMutex * _mutex )
{
    pthread_mutex_lock ( &_mutex->m_mutex );
}

void sq_mutex_leave ( SQMutex * _mutex )
{
    pthread_mutex_unlock ( &_mutex->m_mutex );
}

void sq_mutex_destroy ( SQMutex * _mutex )
{
    pthread_mutex_destroy ( &_mutex->m_mutex );
    free ( _mutex );
}

#endif

#else 

typedef struct _SQMutex
{
   unsigned char unused_parameter;
} _SQMutex;

SQMutex * sq_mutex_create ()
{
    return NULL;
}

void sq_mutex_enter ( SQMutex * _mutex )
{
}

void sq_mutex_leave ( SQMutex * _mutex )
{
}

void sq_mutex_destroy ( SQMutex * _mutex )
{
}

#endif
