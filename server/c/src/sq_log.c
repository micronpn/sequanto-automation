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
#include "sequanto/protocol.h"

static const char SQ_LOG_BANG_LOG[] SQ_CONST_VARIABLE = "!LOG ";
static const char SQ_LOG_ESCAPED_QUOTE[] SQ_CONST_VARIABLE = "\\\"";
static const char SQ_LOG_NEWLINE[] SQ_CONST_VARIABLE = "\r\n";

void sq_log ( const char * _message )
{
    SQServer * server = sq_server_get_instance ();

    sq_stream_enter_write ( server->m_stream );
    sq_stream_write_string ( server->m_stream, sq_get_constant_string(SQ_LOG_BANG_LOG) );
    sq_protocol_write_string ( server->m_stream, _message );
    sq_stream_write_string ( server->m_stream, sq_get_constant_string(SQ_LOG_NEWLINE) );
    sq_stream_exit_write ( server->m_stream );
}

void sq_log_internal_write_string ( SQStream * _stream, const char * _value )
{
    int i = 0;
    for ( i = 0; _value[i] != '\0'; i++ )
    {
        if ( _value[i] == '"' )
        {
            sq_stream_write_string ( _stream, sq_get_constant_string(SQ_LOG_ESCAPED_QUOTE) );
        }
        else if ( _value[i] == '\n' )
        {
            sq_stream_write_byte ( _stream, '\\' );
            sq_stream_write_byte ( _stream, 'n' );
        }
        else if ( _value[i] == '\r' )
        {
            sq_stream_write_byte ( _stream, '\\' );
            sq_stream_write_byte ( _stream, 'r' );
        }
        else
        {
            sq_stream_write_byte ( _stream, _value[i] );
        }
    }
}

void sq_logf ( const char * _format, ... )
{
    va_list lst;
    int i;
    int intValue;
    SQBool boolValue;
    float floatValue;
    void * pointerValue;
    const char * stringValue;
    SQServer * server = sq_server_get_instance ();

    sq_stream_enter_write ( server->m_stream );
    sq_stream_write_string ( server->m_stream, sq_get_constant_string(SQ_LOG_BANG_LOG) );
    sq_stream_write_byte ( server->m_stream, '"' );
    va_start ( lst, _format );

    for ( i = 0; _format[i] != '\0'; i++ )
    {
        if ( _format[i] == '%' )
        {
            i++;
            if ( _format[i] != '\0' )
            {
                switch ( _format[i] )
                {
                case 'd':
                case 'i':
                    intValue = va_arg(lst, int);
                    sq_protocol_write_integer ( server->m_stream, intValue );
                    break;

                case 'f':
                    floatValue = (float) va_arg(lst, double);
                    sq_protocol_write_float ( server->m_stream, floatValue );
                    break;

                case 'b':
                    boolValue = (SQBool) va_arg(lst, int);
                    sq_protocol_write_boolean ( server->m_stream, boolValue );
                    break;

                case 'x':
                case 'p':
                    pointerValue = va_arg(lst, void*);
                    sq_protocol_write_byte_array ( server->m_stream, (SQByte*) &pointerValue, ((SQByte*) &pointerValue) + 4);
                    break;

                case 's':
                    stringValue = (const char*) va_arg(lst, void*);
                    sq_log_internal_write_string ( server->m_stream, stringValue );
                    break;
                }
            }
        }
        else if ( _format[i] == '"' )
        {
            sq_stream_write_string ( server->m_stream, sq_get_constant_string(SQ_LOG_ESCAPED_QUOTE) );
        }
        else
        {
            sq_stream_write_byte ( server->m_stream, _format[i] );
        }
    }
    va_end ( lst );
    sq_stream_write_byte ( server->m_stream, '"' );
    sq_stream_write_string ( server->m_stream, sq_get_constant_string(SQ_LOG_NEWLINE) );
    sq_stream_exit_write ( server->m_stream );
}
