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

#include "config.h"

#include "sequanto/system.h"

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int sq_system_tickcount ()
{
   return GetTickCount();
}

void sq_system_sleep ( int _milliseconds )
{
   Sleep ( _milliseconds );
}

#endif


#ifdef SQ_UNIX

#include <unistd.h>
#include <sys/timeb.h>

int sq_system_tickcount ()
{
  struct timeb tb;
  int ticks;
  ftime ( &tb );
  ticks = tb.time * 1000 + tb.millitm;
  return ticks;
}

void sq_system_sleep (int _milliseconds)
{
  usleep ( _milliseconds * 1000 );
}

#endif


#ifdef SQ_ARDUINO

#include "wiring.h"

int sq_system_tickcount ()
{
  return -1;
}

void sq_system_sleep (int _milliseconds)
{
   delay ( _milliseconds );
}

#endif

