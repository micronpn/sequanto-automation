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

#ifndef SEQUANTO_PROTOCOL_H_
#define SEQUANTO_PROTOCOL_H_

#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>

#ifdef __cplusplus
extern "C"
{
#endif

static const int SQ_PROTOCOL_VERSION = 1;

SQBool sq_protocol_write_type( SQStream * _stream, SQValueType _type );
SQBool sq_protocol_write_integer ( SQStream * _stream, int _value );
SQBool sq_protocol_write_string( SQStream * _stream, const char * const _value );
SQBool sq_protocol_write_string_out( SQStream * _stream, SQStringOut * _value );
SQBool sq_protocol_write_boolean( SQStream * _stream, SQBool _value );
SQBool sq_protocol_write_null ( SQStream * _stream );
SQBool sq_protocol_write_float( SQStream * _stream, float _value );
SQBool sq_protocol_write_byte_array( SQStream * _stream, SQByte * _start, SQByte * _end );

void sq_protocol_write_protocol_message ( SQStream * _stream );
void sq_protocol_write_success_message ( SQStream * _stream );
void sq_protocol_write_success_with_values_message ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues );
void sq_protocol_write_failure_message ( SQStream * _stream );
void sq_protocol_write_failure_with_text_message ( SQStream * _stream, const char * const _text );
void sq_protocol_write_failure_with_values_message ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues );
void sq_protocol_write_update_message ( SQStream * _stream, const char * const _fullname );
void sq_protocol_write_update_with_value_message ( SQStream * _stream, const char * const _fullname , const SQValue * const _value );

void sq_protocol_write_success_with_integer_message ( SQStream * _stream, int _value );
void sq_protocol_write_success_with_string_message ( SQStream * _stream, const char * const _value );
void sq_protocol_write_success_with_string_out_message ( SQStream * _stream, SQStringOut * _value );
void sq_protocol_write_success_with_boolean_message ( SQStream * _stream, SQBool _value );
void sq_protocol_write_success_with_float_message ( SQStream * _stream, float _value );
void sq_protocol_write_success_with_byte_array_message ( SQStream * _stream, SQByte * _start, SQByte * _end );

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_PROTOCOL_H_ */
