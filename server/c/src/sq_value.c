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

#include "sequanto/value.h"
#include "sequanto/protocol.h"

#include <string.h>
#include <stdlib.h>

void sq_value_init ( SQValue * _value )
{
   _value->m_type = VALUE_TYPE_NO_VALUE;
}

void sq_value_integer ( SQValue * _value, int _initialValue)
{
   _value->m_type = VALUE_TYPE_INTEGER;
   _value->Value.m_integerValue = _initialValue;
}

void sq_value_float ( SQValue * _value, float _initialValue)
{
   _value->m_type = VALUE_TYPE_FLOAT;
   _value->Value.m_floatValue = _initialValue;
}

void sq_value_boolean ( SQValue * _value, SQBool _initialValue)
{
   _value->m_type = VALUE_TYPE_BOOLEAN;
   _value->Value.m_booleanValue = _initialValue;
}

void sq_value_string ( SQValue * _value, char * _initialValue)
{
   _value->m_type = VALUE_TYPE_STRING;
   _value->Value.m_stringValue = _initialValue;
}

void sq_value_const_string ( SQValue * _value, const char * _initialValue)
{
   _value->m_type = VALUE_TYPE_CONST_STRING;
   _value->Value.m_constStringValue = _initialValue;
}

#ifdef SQ_ARDUINO
char * strdup ( const char * _value )
{
  char * ret;
  size_t length = strlen(_value);
  ret = (char*) malloc(length + 1);
  memcpy ( ret, _value, length + 1 );
  return ret;
}
#endif

void sq_value_string_copy ( SQValue * _value, const char * const _initialValue)
{
   _value->m_type = VALUE_TYPE_STRING;
#ifdef SQ_WIN32
   _value->Value.m_stringValue = _strdup(_initialValue);
#else
   _value->Value.m_stringValue = strdup(_initialValue);
#endif
}

void sq_value_byte_array ( SQValue * _value, SQByte * _initialValue, size_t _byteArrayLength)
{
   _value->m_type = VALUE_TYPE_BYTE_ARRAY;
   _value->Value.ArrayValue.m_byteArrayValue = _initialValue;
   _value->Value.ArrayValue.m_byteArrayLength = _byteArrayLength;
}

void sq_value_null ( SQValue * _value )
{
   _value->m_type = VALUE_TYPE_NULL;
}

SQBool sq_value_write ( const SQValue * const _value, SQStream * _stream )
{
   switch ( _value->m_type )
   {
   case VALUE_TYPE_INTEGER:
      return sq_protocol_write_integer ( _stream, _value->Value.m_integerValue );

   case VALUE_TYPE_STRING:
      return sq_protocol_write_string ( _stream, _value->Value.m_stringValue );

   case VALUE_TYPE_CONST_STRING:
      return sq_protocol_write_string ( _stream, _value->Value.m_constStringValue );

   case VALUE_TYPE_BOOLEAN:
      return sq_protocol_write_boolean ( _stream, _value->Value.m_booleanValue );

   case VALUE_TYPE_BYTE_ARRAY:
      return sq_protocol_write_byte_array ( _stream, _value->Value.ArrayValue.m_byteArrayValue, _value->Value.ArrayValue.m_byteArrayValue + _value->Value.ArrayValue.m_byteArrayLength );

   case VALUE_TYPE_FLOAT:
      return sq_protocol_write_float ( _stream, _value->Value.m_floatValue );

   case VALUE_TYPE_NULL:
      return sq_protocol_write_null ( _stream );

   default:
      return SQ_FALSE;
   }
}

void sq_value_free ( SQValue * _value )
{
  switch ( _value->m_type )
  {
  case VALUE_TYPE_STRING:
    free ( _value->Value.m_stringValue );
    _value->Value.m_stringValue = NULL;
    break;
    
  case VALUE_TYPE_CONST_STRING:
    _value->Value.m_stringValue = NULL;
    break;
      
  case VALUE_TYPE_BYTE_ARRAY:
    free ( _value->Value.ArrayValue.m_byteArrayValue );
    _value->Value.ArrayValue.m_byteArrayValue = NULL;
    _value->Value.ArrayValue.m_byteArrayLength = 0;
    break;
    
  default:
    break;
  }
  _value->m_type = VALUE_TYPE_NO_VALUE;
}

SQBool sq_values_write ( const SQValue * const _start, size_t _numberOfValues, SQStream * _stream )
{
   const SQValue * it = _start;
   for ( ; it != _start + _numberOfValues; it++ )
   {
      if ( it != _start )
      {
         if ( sq_stream_write_byte ( _stream, ' ' ) == SQ_FALSE ) return SQ_FALSE;
      }
      if ( sq_value_write ( it, _stream ) == SQ_FALSE ) return SQ_FALSE;
   }
   return SQ_TRUE;
}

#define READ_OR_FAIL(_buffer) { _buffer++; if ( *_buffer == '\0' ) return NULL; }

SQByte * sq_parse_identifier ( SQValue * _value, SQByte * _buffer )
{
   if ( *_buffer == 't' )
   {
      sq_value_boolean ( _value, SQ_TRUE );
   }
   else if ( *_buffer == 'f' )
   {
      sq_value_boolean ( _value, SQ_FALSE );
   }
   else if ( *_buffer == 'n' )
   {
      sq_value_null ( _value );
   }
   else
   {
      return NULL;
   }
   while ( *_buffer >= 'a' && *_buffer <= 'z' )
   {
      READ_OR_FAIL ( _buffer );
   }
   return _buffer;
}

static SQ_INLINE SQByte sq_parse_decode_hex_character ( SQByte _character )
{
    SQByte ret = _character - '0';
    if ( ret > 9 )
    {
        ret = 10 + (_character - 'A');
        if ( ret > 15 )
        {
            ret = 10 + (_character - 'A');
        }
    }
    return ret & 0x0F;
}

static SQ_INLINE SQByte sq_parse_decode_two_hex_characters ( SQByte * _start )
{
    SQByte leftHand = sq_parse_decode_hex_character ( *_start );
    SQByte rightHand = sq_parse_decode_hex_character ( *(_start + 1) );
    return ((leftHand << 4) & 0xF0) | (rightHand & 0x0F);
}

SQByte * sq_parse_byte_array ( SQValue * _value, SQByte * _buffer )
{
    int i = 0;
    int numBytes = 0;
    SQByte * bytes;
    SQByte * start = _buffer;
    while ( *_buffer != ' ' && *_buffer != '\0' && *_buffer != '\r' && *_buffer != '\n' )
    {
        numBytes ++;
        _buffer ++;
    }
    
    // If numBytes is even then have received only "half" of the last byte.
    if ( (numBytes & 1) != 0 )
    {
        SQ_LOG0("Received an un-even number of hexadecimal characters when parsing byte array.");
        return NULL;
    }
    
    numBytes = numBytes / 2;
    
    bytes = malloc ( numBytes );
    for ( ; start != _buffer; start += 2 )
    {
        bytes[i] = sq_parse_decode_two_hex_characters ( start );
        i++;
    }
    sq_value_byte_array ( _value, bytes, numBytes );
    
    return _buffer;
}

SQByte * sq_parse_integer_or_float ( SQValue * _value, SQByte * _buffer )
{
   SQBool negate = SQ_FALSE;
   int value = 0;
   float floatValue = 0;

   if ( *_buffer == '-' )
   {
      negate = SQ_TRUE;

      READ_OR_FAIL ( _buffer );
   }

   while ( (*_buffer >= '0' && *_buffer <= '9') )
   {
      value = value * 10 + (*_buffer - '0');

      READ_OR_FAIL( _buffer );
   }
   if ( *_buffer == '.' )
   {
      READ_OR_FAIL ( _buffer );

      floatValue = (float) value;
      value = 10;
      while ( *_buffer >= '0' && *_buffer <= '9' )
      {
         floatValue = floatValue + ((float) (*_buffer - '0')) / ((float) value);

         READ_OR_FAIL(_buffer);
         value = value * 10;
      }
      if ( negate == SQ_TRUE )
      {
         floatValue = -floatValue;
      }
     sq_value_float ( _value, floatValue );
   }
   else
   {
      if ( negate == SQ_TRUE )
      {
         value = -value;
      }
      sq_value_integer ( _value, value );
   }
   return _buffer;
}

SQByte * sq_parse_string ( SQValue * _value, SQByte * _buffer )
{
   size_t i = 0;
   SQBool escaped = SQ_FALSE;

   char parsedString[SQ_MAX_STRING_LENGTH];
   memset ( &parsedString, 0, SQ_MAX_STRING_LENGTH );

   READ_OR_FAIL(_buffer);
   while ( (*_buffer != '"' || escaped == SQ_TRUE) )
   {
      if ( escaped == SQ_TRUE )
      {
         if ( *_buffer == 'r' )
         {
            parsedString[i] = '\r';
         }
         else if ( *_buffer == 'n' )
         {
            parsedString[i] = '\n';
         }
         else
         {
            parsedString[i] = *_buffer;
         }
         i++;
         escaped = SQ_FALSE;
      }
      else if ( *_buffer == '\\' )
      {
         escaped = SQ_TRUE;
      }
      else
      {
         parsedString[i] = *_buffer;
         i++;
      }
      READ_OR_FAIL(_buffer);
   }
   sq_value_string_copy ( _value, (const char * const) &parsedString );
   return _buffer;
}

#undef READ_OR_FAIL

size_t sq_values_parse ( SQValue * _start, size_t _maximumValues, SQByte * _buffer )
{
   size_t valuesRead = 0;

   while ( valuesRead < _maximumValues && *_buffer != '\n' && *_buffer != '\r' )
   {
      while ( *_buffer == ' ' )
      {
         _buffer ++;
         if ( *_buffer == '\0' )
         {
            return valuesRead;
         }
      }
      if ( *_buffer == '"' )
      {
         _buffer = sq_parse_string ( _start + valuesRead, _buffer );
         if ( _buffer == NULL )
         {
            return valuesRead;
         }
         valuesRead ++;
         _buffer++;
         if ( *_buffer == '\0' )
         {
            return valuesRead;
         }
      }
      else if ( (*_buffer >= '0' && *_buffer <= '9') || *_buffer == '-' )
      {
          if ( *(_buffer + 1) == 'x' )
          {
              _buffer = sq_parse_byte_array ( _start + valuesRead, _buffer + 2 );
              if ( _buffer == NULL )
              {
                  return valuesRead;
              }
              valuesRead ++;
          }
          else
          {
              _buffer = sq_parse_integer_or_float ( _start + valuesRead, _buffer );
              if ( _buffer == NULL )
              {
                  return valuesRead;
              }
              valuesRead ++;
          }
      }
      else if ( *_buffer >= 'a' && *_buffer <= 'z' )
      {
         _buffer = sq_parse_identifier ( _start + valuesRead, _buffer );
         if ( _buffer == NULL )
         {
            return valuesRead;
         }
         valuesRead ++;
      }
      else
      {
         return valuesRead;
      }
   }
   if ( *_buffer != '\n' && *_buffer != '\r' )
   {
       // Return + 1 to indicate that we did not have enough space to parse all the parameters given.
       return valuesRead + 1;
   }
   else
   {
       return valuesRead;
   }
}

#undef READ_OR_FAIL
