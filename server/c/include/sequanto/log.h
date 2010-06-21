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

#ifndef SEQUANTO_LOG_H_
#define SEQUANTO_LOG_H_

#include <sequanto/log.h>
#include <stdarg.h>

/**
 * \file log.h
 * \brief Write log message to the client.
 * 
 * These functions provides a simple way for the server (target) to
 * send logging statements to any conneted client.
 * 
 * These log messages are sent to the client using the !LOG
 * asynchronous message introduced in revision 2 of the automation
 * protocol.
 */

#ifdef __cplusplus
extern "C"
{
#endif
   
   /**
    * Write a log message to the connected clients (if any).
    * 
    * The message will automatically be escaped such that it adheres to the automation protocol.
    * 
    * @param _message The message to write.
    */
   void sq_log ( const char * _message );

   /**
    * Write a log message to the connected clients (if any) using a
    * printf-like syntax.
    * 
    * Generally you can safely just assume that normal printf
    * statements work, e.g. %d and %i means integer, %f means float
    * (double) and %s means string.
    * 
    * Currently the accepted statements are:
    * 
    *    %d, %i: Write an integer
    *    %f: Write a double or a float
    *    %b: Write a SQBool.
    *    %x, %p: Write an integer as hexadecimal (useful when memory addresses).
    *    %s: Write a null terminated string.
    *
    * @param _format The message to write, including any printf-like statements.
    */
   void sq_logf ( const char * _format, ... );
   
#ifdef __cplusplus
}
#endif

#endif
