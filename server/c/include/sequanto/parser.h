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

#include <sequanto/macros.h>
#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>

/**
 * @defgroup parser Parser
 * @ingroup c
 * 
 * The parser will evaluate automation commands as they as fed into it
 * using sq_parser_input_byte. The parser will then call the functions
 * mentioned in the list of application functions, which can be
 * implemented by the user to handle requests.
 * 
 * @see application
 * 
 * @{
 */

/**
 * The length of the internal buffer in the parser, this means that
 * the parser can only handle requests with a length shorter or equal
 * to SQ_BUFFER_SIZE.
 */
/* #define SQ_BUFFER_SIZE 1024 */
#define SQ_BUFFER_SIZE 256

SQ_BEGIN_DECL

/**
 * The actual parser struct, you should never have to access the members of the struct directly.
 */
typedef struct _SQParser
{
   size_t m_inputBufferPosition;
   SQByte m_inputBuffer[SQ_BUFFER_SIZE];
} SQParser;

/**
 * Initialize the parser struct.
 */
SQ_DECL void sq_parser_init ( SQParser * _parser );

/**
 * Pass one byte into the parser, if the buffer contains a request
 * after the byte is added, the request will be evaluated and the
 * appropriate application function will be called.
 *
 * @see application
 */
SQ_DECL void sq_parser_input_byte ( SQParser * _parser, SQStream * _outputStream, SQByte _byte );

/**
 * Destroy the parser struct.
 */
SQ_DECL void sq_parser_destroy ( SQParser * _parser );

/**
 * @}
 */

/**
 * @defgroup application Application functions
 * 
 * The sq_parser_info, _list, _call, _property_set and _property_get should be implemented in the application.
 * 
 * Normally these are just implemented by generating them using generate_automation_defines.py
 * 
 * @ingroup c
 */
/*@{*/

/**
 * @see application
 */ 
void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );
/**
 * @see application
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
/**
 * @see sq_parser_info
 */
void sq_parser_enable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );
/**
 * @see sq_parser_info
 */
void sq_parser_disable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath );

/*@}*/

SQ_END_DECL

#endif
