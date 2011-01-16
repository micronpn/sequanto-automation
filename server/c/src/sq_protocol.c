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

#include <math.h>
#include <stddef.h>

#include "config.h"

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#include "sequanto/types.h"
#include "sequanto/protocol.h"

static char NEWLINE[] SQ_CONST_VARIABLE = "\r\n";

SQBool sq_protocol_write_type( SQStream * _stream, SQValueType _type )
{
   switch ( _type )
   {
   case VALUE_TYPE_INTEGER:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("Integer")) );

   case VALUE_TYPE_BOOLEAN:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("Boolean")) );

   case VALUE_TYPE_STRING:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("String")) );

   case VALUE_TYPE_FLOAT:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("Float")) );

   case VALUE_TYPE_BYTE_ARRAY:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("ByteArray")) );

   case VALUE_TYPE_VOID:
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("Void")) );

   default:
      return SQ_FALSE;
   }
}

#define WRITE_BYTE_OR_FAIL(byte) if ( sq_stream_write_byte(_stream, (byte)) == SQ_FALSE ) return SQ_FALSE;

SQBool sq_protocol_write_integer ( SQStream * _stream, int _value )
{
   char buffer[SQ_MAX_VALUE_LENGTH];
   int i = 0;

   if ( _value < 0 )
   {
      /* If this is true we have hit the minimum value of the integer, and revert to just returning 0. */
      if ( _value == -_value )
      {
         _value = 0;
      }
      else
      {
         WRITE_BYTE_OR_FAIL ( '-' );
         _value = -_value;
      }
   }

   if ( _value == 0 )
   {
      return sq_stream_write_byte ( _stream, '0' );
   }

   while ( _value > 0 )
   {
      buffer[i] = (char) ('0' + (_value % 10));

      i++;

      _value = _value / 10;
   }
   for ( i--; i >= 0; i-- )
   {
      WRITE_BYTE_OR_FAIL ( buffer[i] );
   }
   return SQ_TRUE;
}

SQBool sq_protocol_write_string( SQStream * _stream, const char * const _value )
{
   size_t i = 0;
   char ch;

   WRITE_BYTE_OR_FAIL( '"' );

   for ( ; (ch = _value[i]) != '\0'; i++ )
   {
      if ( ch == '\n' )
      {
				WRITE_BYTE_OR_FAIL ( '\\' );
				WRITE_BYTE_OR_FAIL ( 'n' );
      }
      else if ( ch == '\r' )
      {
				WRITE_BYTE_OR_FAIL ( '\\' );
				WRITE_BYTE_OR_FAIL ( 'r' );
      }
      else if ( ch >= ' ' )
      {
         if ( ch == '\\' || ch == '"' )
         {
            WRITE_BYTE_OR_FAIL ( '\\' );
         }
         WRITE_BYTE_OR_FAIL ( ch );
      }
   }
   WRITE_BYTE_OR_FAIL ( '"' );

   return SQ_TRUE;
}

SQBool sq_protocol_write_string_out( SQStream * _stream, SQStringOut * _value )
{
	WRITE_BYTE_OR_FAIL( '"' );
	while (_value->HasMore(_value))
	{
		char ch = _value->GetNext(_value);
		if ( ch == '\n' )
		{
			WRITE_BYTE_OR_FAIL ( '\\' );
			WRITE_BYTE_OR_FAIL ( 'n' );
		}
		else if ( ch == '\r' )
		{
			WRITE_BYTE_OR_FAIL ( '\\' );
			WRITE_BYTE_OR_FAIL ( 'r' );
		}
		else if ( ch >= ' ' )
		{
			 if ( ch == '\\' || ch == '"' )
			 {
					WRITE_BYTE_OR_FAIL ( '\\' );
			 }
			 WRITE_BYTE_OR_FAIL ( ch );
		}
	}
	WRITE_BYTE_OR_FAIL( '"' );
	return SQ_TRUE;
}

SQBool sq_protocol_write_boolean ( SQStream * _stream, SQBool _value )
{
   if ( _value == SQ_TRUE )
   {
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("true")) );
   }
   else
   {
      return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("false")) );
   }
}

SQBool sq_protocol_write_null ( SQStream * _stream )
{
   return sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("null")) );
}

SQBool sq_protocol_write_float( SQStream * _stream, float _value )
{
   char buffer[SQ_MAX_VALUE_LENGTH];
#ifdef HAVE_SNPRINTF
#  ifdef HAVE_SNPRINTF_S
   
   int i;
   SNPRINTF_S_FUNCTION ( buffer, SQ_MAX_VALUE_LENGTH - 1, SQ_MAX_VALUE_LENGTH, "%f", _value );
   for ( i = 0; buffer[i] != 0 && i < SQ_MAX_VALUE_LENGTH; i++ )
   {
      WRITE_BYTE_OR_FAIL ( buffer[i] );
   }
   return SQ_TRUE;

#  else

   int i;
   SNPRINTF_FUNCTION ( buffer, SQ_MAX_VALUE_LENGTH - 1, "%f", (double) _value );
   for ( i = 0; buffer[i] != 0 && i < SQ_MAX_VALUE_LENGTH; i++ )
   {
      WRITE_BYTE_OR_FAIL ( buffer[i] );
   }
   return SQ_TRUE;

#  endif
#else
   int i = 0, tempValue;

   if ( _value < 0.0f )
   {
      WRITE_BYTE_OR_FAIL ( '-' );
      _value = -_value;
   }

   tempValue = (int) _value;
   while ( tempValue != 0 && i < SQ_MAX_VALUE_LENGTH )
   {
      buffer[i] = (char) ('0' + (tempValue % 10));
      i++;

      tempValue = tempValue / 10;
   }
   for ( i--; i >= 0; i-- )
   {
      WRITE_BYTE_OR_FAIL ( buffer[i] );
   }

   WRITE_BYTE_OR_FAIL ( '.' );

   _value = _value - ((int)_value);
   tempValue = (int) (_value * 1000000);
   if ( tempValue == 0 )
   {
      WRITE_BYTE_OR_FAIL ( '0' );
   }
   else
   {
      i = 0;
      while ( tempValue != 0 && i < 11)
      {
         buffer[i] = (char) ('0' + (tempValue % 10));
         i++;

         tempValue = tempValue / 10;
      }
      for ( i--; i >= 0; i-- )
      {
         WRITE_BYTE_OR_FAIL ( buffer[i] );
      }
   }
   return SQ_TRUE;
#endif
}

SQBool sq_protocol_write_byte_array( SQStream * _stream, SQByte * _start, SQByte * _end )
{
   SQByte *it = _start;

   sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("0x")) );
   
   for ( ; it != _end; it ++ )
   {
      if ( ((*it  >> 4 ) & 0x0F) > 9 )
      {
         WRITE_BYTE_OR_FAIL ( 'A' + (((*it) >> 4) & 0x0F) - 10 );
      }
      else
      {
         WRITE_BYTE_OR_FAIL ( '0' + (((*it) >> 4) & 0x0F) );
      }

      if ( ((*it) & 0x0F) > 9 )
      {
         WRITE_BYTE_OR_FAIL ( 'A' + ((*it) & 0x0F) - 10 );
      }
      else
      {
         WRITE_BYTE_OR_FAIL ( '0' + ((*it) & 0x0F) );
      }
   }
   return SQ_TRUE;
}

void sq_protocol_write_protocol ( SQStream * _stream )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+PROTOCOL ")) );
   sq_protocol_write_integer ( _stream, SQ_PROTOCOL_VERSION  );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_success ( SQStream * _stream )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_byte ( _stream, '+' );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_success_with_values ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_byte ( _stream, '+' );
   sq_values_write ( _value, _numberOfValues, _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_failure ( SQStream * _stream )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_byte ( _stream, '-' );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_failure_with_text ( SQStream * _stream, const char * const _text )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_byte ( _stream, '-' );
   sq_protocol_write_string ( _stream, _text );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_failure_with_values ( SQStream * _stream, const SQValue * const _value, size_t _numberOfValues )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_byte ( _stream, '-' );
   sq_values_write ( _value, _numberOfValues, _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_protocol_write_update( SQStream * _stream, const char * const _fullname )
{
   sq_protocol_write_update_with_value ( _stream, _fullname, NULL );
}

void sq_protocol_write_update_with_value( SQStream * _stream, const char * const _fullname, const SQValue * const _value )
{
   sq_stream_enter_write ( _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("!UPDATE ")) );
   sq_stream_write_string ( _stream, _fullname );
   if ( _value != NULL )
   {
      sq_stream_write_byte ( _stream, ' ' );
      sq_value_write( _value, _stream );
   }
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}
