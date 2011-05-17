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

#include <sequanto/macros.h>
#include <sequanto/types.h>

SQ_BEGIN_DECL

/**
 * @defgroup thread Threading and mutexes.
 * 
 * Wrapper around the platform specific threading primitives.
 * 
 * On platforms which do not support threads and mutexes (embedded
 * platforms) they will simply do nothing.
 *
 * @ingroup c
 * @{
 */

/**
 * Thread pimpl.
 */
typedef struct _SQThread SQThread;

/**
 * The prototype for functions which should be run in another thread.
 */
typedef void (*SQThreadRunFunction) ( SQThread * _thread, void * _data );

/**
 * Determine on runtime if the current platform supports threading.
 */
SQ_DECL SQBool sq_thread_is_supported ( void );

/**
 * Create a thread which will run the function _function in a new
 * thread when sq_thread_start is called.
 * 
 * The _data will be passed as a parameter to the _function when the
 * thread is started.
 */
SQ_DECL SQThread * sq_thread_create ( SQThreadRunFunction _function, void * _data );

/**
 * Start the given thread.
 */
SQ_DECL void sq_thread_start ( SQThread * _thread );

/**
 * Destroy the given thread.
 */
SQ_DECL void sq_thread_destroy ( SQThread * _thread );

/**
 * Mutex pimpl.
 */
typedef struct _SQMutex SQMutex;

/**
 * Create a new mutex.
 */
SQ_DECL SQMutex * sq_mutex_create ( void );

/**
 * Lock the mutex.
 */
SQ_DECL void sq_mutex_enter ( SQMutex * _mutex );

/**
 * Unlock the mutex.
 */
SQ_DECL void sq_mutex_leave ( SQMutex * _mutex );

/**
 * Destroy the mutex.
 */
SQ_DECL void sq_mutex_destroy ( SQMutex * _mutex );

/**
 * @}
 */

SQ_END_DECL

#endif /* SEQUANTO_THREAD_H_ */
