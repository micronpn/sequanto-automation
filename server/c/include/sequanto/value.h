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

#ifndef SEQUANTO_VALUE_H_
#define SEQUANTO_VALUE_H_

#include <sequanto/macros.h>
#include <sequanto/types.h>
#include <sequanto/stream.h>

SQ_BEGIN_DECL

typedef enum
{
	VALUE_TYPE_NO_VALUE,
	VALUE_TYPE_INTEGER,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_BOOLEAN,
	VALUE_TYPE_STRING,
	VALUE_TYPE_SQSTRINGOUT = VALUE_TYPE_STRING,
	VALUE_TYPE_BYTE_ARRAY,
	VALUE_TYPE_NULL,
    VALUE_TYPE_VOID,
	VALUE_TYPE_CONST_STRING
} SQValueType;

typedef struct
{
	SQValueType m_type;
   union
   {
      int m_integerValue;
      float m_floatValue;
      SQBool m_booleanValue;
      char * m_stringValue;
      const char * m_constStringValue;
      struct
      {
         SQByte * m_byteArrayValue;
         size_t m_byteArrayLength;
      } ArrayValue;
   } Value;
} SQValue;

/**
 * Initialize the SQValue as having no value.
 */
SQ_DECL void sq_value_init ( SQValue * _value );

/**
 * Initialize the SQValue as an integer.
 */
SQ_DECL void sq_value_integer ( SQValue * _value, int _initialValue);

/**
 * Initialize the SQValue as a float.
 */
SQ_DECL void sq_value_float ( SQValue * _value, float _initialValue);

/**
 * Initialize the SQValue as a boolean.
 */
SQ_DECL void sq_value_boolean ( SQValue * _value, SQBool _initialValue);

/**
 * Initialize the SQValue as a string.
 *
 * Please note that the given value will _not_ be copied, the string will be free'ed when sq_value_free(...) is called.
 */
SQ_DECL void sq_value_string ( SQValue * _value, char * _initialValue);

/**
 * Initialize the SQValue as a const string.
 *
 * Please note that the given value will _not_ be copied, the string will have to be valid while this SQValue is valid.
 * The value is _not_ free'ed when sq_value_free(...) is called.
 */
SQ_DECL void sq_value_const_string ( SQValue * _value, const char * _initialValue);

/**
 * Initialize the SQValue as a string.
 *
 * Please note that the given value will be copied and the string will be free'ed when sq_value_free(...) is called.
 */
SQ_DECL void sq_value_string_copy ( SQValue * _value, const char * const _initialValue);

/**
 * Initialize the SQValue as a byte array.
 *
 * The initial value is _not_ copied, and will be freed when the sq_value_free is called.
 *
 * @see sq_value_string
 */
SQ_DECL void sq_value_byte_array ( SQValue * _value, SQByte * _initialValue, size_t _byteArrayLength );

/**
 * Initialize the SQValue as a null.
 */
SQ_DECL void sq_value_null ( SQValue * _value );

/**
 * Write the given value to the given stream.
 */
SQ_DECL SQBool sq_value_write ( const SQValue * const _value, SQStream * _stream );

/**
 * Free any memory that the given SQValue is consuming (string or byte array) and set the type to "NO VALUE".
 */
SQ_DECL void sq_value_free ( SQValue * _value );

/**
 * Write values from _start to _start + _numberOfValues separated by space.
 */
SQ_DECL SQBool sq_values_write ( const SQValue * const _start, size_t _numberOfValues, SQStream * _stream );

/**
 * Parse a list of values separated by spaces and terminated by \r\n from _buffer.
 *
 * @return the number of values read.
 */
SQ_DECL size_t sq_values_parse ( SQValue * _start, size_t _maximumValues, SQByte * _buffer );

SQ_END_DECL

#endif /* SEQUANTO_VALUE_H_ */
