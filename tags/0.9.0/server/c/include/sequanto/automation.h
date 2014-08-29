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

#ifndef SEQUANTO_AUTOMATION_H_
#define SEQUANTO_AUTOMATION_H_

#include <sequanto/macros.h>
#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/server.h>
#include <sequanto/system.h>
#include <sequanto/log.h>

SQ_BEGIN_DECL

/**
 * @defgroup c C part of the library
 */

/**
 * @defgroup internal Internal functions, should not be called by user code.
 */

/**
 * @defgroup init Initializing the library.
 * @ingroup c
 */

/**
 * Initialize the sequanto-automation library. This should always be
 * the first thing you call before using the library.
 * 
 * @ingroup init
 */
SQ_DECL void sq_init ( void );

/**
 * De-initializes the sequanto-automation library. This should always
 * be the last thing you call. Usually this is registered to be called
 * with something like the atexit(...) system call..
 * 
 * @ingroup init
 */
SQ_DECL void sq_shutdown ( void );

SQ_END_DECL

#endif /* SEQUANTO_AUTOMATION_H_ */
