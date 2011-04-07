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

#ifndef SEQUANTO_SYSTEM_H_
#define SEQUANTO_SYSTEM_H_

#include <sequanto/macros.h>

SQ_BEGIN_DECL

SQ_DECL int sq_system_tickcount ( void );
SQ_DECL void sq_system_sleep ( int _milliseconds );

#define SQ_MAX(a,b) ((a) < (b) ? (b) : (a))

SQ_END_DECL

#endif /* SEQUANTO_SYSTEM_H_ */
