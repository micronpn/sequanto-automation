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

#ifndef SEQUANTO_PARSER_H_
#define SEQUANTO_PARSER_H_

#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>

//#define SQ_BUFFER_SIZE 1024
#define SQ_BUFFER_SIZE 256

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _SQParser
{
   size_t m_inputBufferPosition;
   SQByte m_inputBuffer[SQ_BUFFER_SIZE];
} SQParser;

void sq_parser_init ( SQParser * _parser );
void sq_parser_input_byte ( SQParser * _parser, SQStream * _outputStream, SQByte _byte );
void sq_parser_destroy ( SQParser * _parser );


/**
 * @defgroup Application Application functions
 */
/*@{*/

/**
 * The sq_parser_info, _list, _call, _property_set and _property_get should be implemented in the application.
 * 
 * Normally these are just implemented by generating them using generate_automation_defines.py
 * 
 */ 
void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );
/**
 * @see sq_parser_info
 */
void sq_parser_list ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );
/**
 * @see sq_parser_info
 */
void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _parameters, int _numberOfParameters );
/**
 * @see sq_parser_info
 */
void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );
/**
 * @see sq_parser_info
 */
void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _value );

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
