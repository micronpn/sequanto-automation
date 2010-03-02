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

#ifndef SEQUANTO_THREAD_H_
#define SEQUANTO_THREAD_H_

#include <sequanto/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _SQThread SQThread;

typedef void (*SQThreadRunFunction) ( SQThread * _thread, void * _data );

/**
 * Determine on runtime if the current platform supports threading.
 */
SQBool sq_thread_is_supported ();

SQThread * sq_thread_create ( SQThreadRunFunction _function, void * _data );
void sq_thread_start ( SQThread * _thread );
void sq_thread_destroy ( SQThread * _thread );

typedef struct _SQMutex SQMutex;

SQMutex * sq_mutex_create ();
void sq_mutex_enter ( SQMutex * _mutex );
void sq_mutex_leave ( SQMutex * _mutex );
void sq_mutex_destroy ( SQMutex * _mutex );

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_THREAD_H_ */
