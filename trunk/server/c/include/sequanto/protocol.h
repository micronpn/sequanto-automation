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

#include <sequanto/macros.h>
#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>

SQ_BEGIN_DECL

/**
 * @defgroup protocol Write automation messages to a stream.
 * 
 * Provides a central place for all functions which can write properly
 * formatted automation messages to a stream.
 * 
 * @ingroup c
 * @{
 */

/**
 * Version of the automation protocol support by this library, see the
 * sequanto-automation library for a list of versions of the
 * automation protocol.
 *
 * @link http://code.google.com/p/sequanto-automation/
 */
static const int SQ_PROTOCOL_VERSION = 1;

SQ_DECL SQBool sq_protocol_write_type( SQStream * _stream, SQValueType _type );
SQ_DECL SQBool sq_protocol_write_integer ( SQStream * _stream, int _value );
SQ_DECL SQBool sq_protocol_write_string( SQStream * _stream, const char * const _value );
SQ_DECL SQBool sq_protocol_write_string_out( SQStream * _stream, SQStringOut * _value );
SQ_DECL SQBool sq_protocol_write_boolean( SQStream * _stream, SQBool _value );
SQ_DECL SQBool sq_protocol_write_null ( SQStream * _stream );
SQ_DECL SQBool sq_protocol_write_float( SQStream * _stream, float _value );
SQ_DECL SQBool sq_protocol_write_byte_array( SQStream * _stream, const SQByte * _start, const SQByte * _end );

/**
 * Writes a response to a PROTOCOL request.
 */
SQ_DECL void sq_protocol_write_protocol_message ( SQStream * _stream );

/**
 * Writes a successful response without values.
 */
SQ_DECL void sq_protocol_write_success_message ( SQStream * _stream );

/**
 * Writes a successful response with the given values.
 */
SQ_DECL void sq_protocol_write_success_with_values_message ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues );

/**
 * Writes a failure response without values.
 */
SQ_DECL void sq_protocol_write_failure_message ( SQStream * _stream );

/**
 * Write a failure response with a single string value.
 */
SQ_DECL void sq_protocol_write_failure_with_text_message ( SQStream * _stream, const char * const _text );

/**
 * Write a failure response with the given values.
 */
SQ_DECL void sq_protocol_write_failure_with_values_message ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues );

/**
 * Writes an asynchronous !UPDATE message for the object with the given full name without values (usually this is done for lists).
 */
SQ_DECL void sq_protocol_write_update_message ( SQStream * _stream, const char * const _fullname );

/**
 * Writes an asynchronous !UPDATE message for the object with the given full name with the given value (usually this is done for properties and monitors).
 */
SQ_DECL void sq_protocol_write_update_with_value_message ( SQStream * _stream, const char * const _fullname , const SQValue * const _value );

/**
 * Writes an asynchronous !UPDATE message for the object with the given full name with the given values (usually this is done for monitors).
 */
SQ_DECL void sq_protocol_write_update_with_values_message ( SQStream * _stream, const char * const _fullname , const SQValue * const _values, size_t _numberOfValues );

/**
 * Writes an asynchronous !ADD message for the object with the given full name.
 */
SQ_DECL void sq_protocol_write_add_message ( SQStream * _stream, const char * const _fullname );

/**
 * Writes an asynchronous !REMOVE message for the object with the given full name.
 */
SQ_DECL void sq_protocol_write_remove_message ( SQStream * _stream, const char * const _fullname );

/**
 * Write successful response with an integer value.
 */
SQ_DECL void sq_protocol_write_success_with_integer_message ( SQStream * _stream, int _value );

/**
 * Write successful response with a string value.
 */
SQ_DECL void sq_protocol_write_success_with_string_message ( SQStream * _stream, const char * const _value );

/**
 * Write successful response with a string value (given as a SQStringOut value).
 */
SQ_DECL void sq_protocol_write_success_with_string_out_message ( SQStream * _stream, SQStringOut * _value );

/**
 * Write successful response with a boolean value.
 */
SQ_DECL void sq_protocol_write_success_with_boolean_message ( SQStream * _stream, SQBool _value );

/**
 * Write successful response with a floting point value.
 */
SQ_DECL void sq_protocol_write_success_with_float_message ( SQStream * _stream, float _value );

/**
 * Write successful response with a byte array value.
 */
SQ_DECL void sq_protocol_write_success_with_byte_array_message ( SQStream * _stream, SQByte * _start, SQByte * _end );

SQ_DECL void sq_protocol_treedump_write_start (  SQStream * _stream );

SQ_DECL void sq_protocol_treedump_write_list_begin (  SQStream * _stream, const char * const _name );
SQ_DECL void sq_protocol_treedump_write_list_end (  SQStream * _stream );

SQ_DECL void sq_protocol_treedump_write_other_begin (  SQStream * _stream, const char * const _name );
SQ_DECL void sq_protocol_treedump_write_other_values (  SQStream * _stream );
SQ_DECL void sq_protocol_treedump_write_other_end (  SQStream * _stream );

/**
 * @}
 */

SQ_END_DECL

#endif /* SEQUANTO_PROTOCOL_H_ */
