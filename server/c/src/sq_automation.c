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

#include "sequanto/automation.h"
#include "sequanto/types.h"

#ifdef SQ_USE_WINSOCK

#include <winsock2.h>
static WSADATA wsaData;

void sq_init ()
{
   WSAStartup ( MAKEWORD(2,0), &wsaData );
}

void sq_shutdown ()
{
   WSACleanup ();
}

#else

#ifdef SQ_ARDUINO

#include "wiring.h"

void sq_init ()
{
    init();
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void sq_blink ()
{
    digitalWrite(13, HIGH);
    sq_system_sleep ( 500 );
    digitalWrite(13, LOW);
    sq_system_sleep ( 500 );
}

void sq_shutdown ()
{
}

#else

void sq_init ()
{
}

void sq_shutdown ()
{
}

#endif

#endif
