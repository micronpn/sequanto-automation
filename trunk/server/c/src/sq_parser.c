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

#include <string.h>

#include "config.h"

#ifdef SQ_USE_ASSERT
#include <assert.h>
#endif

#include "sequanto/parser.h"
#include "sequanto/protocol.h"
#include "sequanto/log.h"

void sq_parser_init ( SQParser * _parser )
{
    SQ_ASSERT ( _parser != NULL );
    
    memset ( _parser->m_inputBuffer, 0, SQ_BUFFER_SIZE );
    _parser->m_inputBufferPosition = 0;
}

size_t sq_parser_internal_read_object_path ( SQParser * _parser, size_t _startIndex )
{
    SQ_ASSERT ( _parser != NULL );
    
    while ( _parser->m_inputBuffer[_startIndex] != '\0'
            &&
            _startIndex < SQ_MAX_OBJECT_LENGTH
            &&
            ( (_parser->m_inputBuffer[_startIndex] >= 'a' && _parser->m_inputBuffer[_startIndex] <= 'z') ||
              (_parser->m_inputBuffer[_startIndex] >= '0' && _parser->m_inputBuffer[_startIndex] <= '9') ||
              (_parser->m_inputBuffer[_startIndex] >= 'A' && _parser->m_inputBuffer[_startIndex] <= 'Z') ||
              _parser->m_inputBuffer[_startIndex] == '/' ||
              _parser->m_inputBuffer[_startIndex] == '-' ||
              _parser->m_inputBuffer[_startIndex] == '_' ||
              _parser->m_inputBuffer[_startIndex] == '.') 
            )
    {
        _startIndex ++;
    }

    return _startIndex;
}

#define SKIP_TO_WHITESPACE() while ( _parser->m_inputBuffer[index] != '\0' && _parser->m_inputBuffer[index] != ' ' && _parser->m_inputBuffer[index] != '\t' ) { index++; };
#define SKIP_WHITESPACE() while ( _parser->m_inputBuffer[index] != '\0' && (_parser->m_inputBuffer[index] == ' ' || _parser->m_inputBuffer[index] == '\t') ) { index++; };

void sq_parser_internal_free_values ( SQValue * _values )
{
   size_t i = 0;
    for ( ; i < SQ_MAX_PARAMETERS; i++ )
    {
        sq_value_free ( &_values[i] );
    }
}

void sq_parser_internal_parse_input_buffer ( SQParser * _parser, SQStream * _outputStream )
{
    SQValue values[SQ_MAX_PARAMETERS];
    size_t i, endOfObjectPath, index = 0;

    SQ_ASSERT ( _parser != NULL );
    
    for ( i = 0; i < SQ_MAX_PARAMETERS; i++ )
    {
        sq_value_init ( &values[i] );
    }
    
    switch ( _parser->m_inputBuffer[0] )
    {
    case 'P': /* PROTOCOL */
        sq_protocol_write_protocol_message(_outputStream);
        break;
        
    case 'G': /* GET */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );

        i = sq_values_parse ( values, SQ_MAX_PARAMETERS, _parser->m_inputBuffer + endOfObjectPath );
        if ( i != 0 )
        {
            sq_protocol_write_failure_with_text_message ( _outputStream, sq_get_constant_string(SQ_STRING_CONSTANT("GET request must be called without values")) );
            sq_parser_internal_free_values ( values );
            return;
        }
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "GET %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_property_get ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index );
        break;
        
    case 'S': /* SET */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();

        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );
        
        i = sq_values_parse ( values, SQ_MAX_PARAMETERS, _parser->m_inputBuffer + endOfObjectPath );
        if ( i != 1 )
        {
            sq_protocol_write_failure_with_text_message ( _outputStream, sq_get_constant_string(SQ_STRING_CONSTANT("SET request must be called with one value")) );
            sq_parser_internal_free_values ( values );
            return;
        }
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "SET %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_property_set ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index, values );
        break;
        
    case 'C': /* CALL */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );

        i = sq_values_parse ( values, SQ_MAX_PARAMETERS, _parser->m_inputBuffer + endOfObjectPath );
        if ( i > SQ_MAX_PARAMETERS )
        {
            sq_protocol_write_failure_with_text_message ( _outputStream, sq_get_constant_string(SQ_STRING_CONSTANT("Too many values given in CALL request")) );
            sq_parser_internal_free_values ( values );
            return;
        }
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "CALL %s\n",  _parser->m_inputBuffer + index );
        
        sq_parser_call ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index, values, i  );
        break;
        
    case 'L': /* LIST */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "LIST %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_list ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index );
        break;
        
    case 'I': /* INFO */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "INFO %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_info ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index );
        break;

    case 'E': /* ENABLE */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "ENABLE %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_enable ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index );
        break;
        
    case'D': /* DISABLE */
        SKIP_TO_WHITESPACE ();
        SKIP_WHITESPACE ();
        
        endOfObjectPath = sq_parser_internal_read_object_path ( _parser, index );
        
        _parser->m_inputBuffer[endOfObjectPath] = '\0';
        
        SQ_LOG1 ( "DISABLE %s\n", _parser->m_inputBuffer + index );
        
        sq_parser_disable ( _parser, _outputStream, (const char * const) _parser->m_inputBuffer + index );
        break;
        
    default:
        sq_logf ( "Bad command '%s'", _parser->m_inputBuffer );
        sq_protocol_write_failure_with_text_message ( _outputStream, sq_get_constant_string(SQ_STRING_CONSTANT("Unrecognized command.")) );
        break;
    }

    sq_parser_internal_free_values ( values );
}

void sq_parser_input_byte ( SQParser * _parser, SQStream * _outputStream, SQByte _byte )
{
    SQ_ASSERT ( _parser != NULL );
    
    if ( _parser->m_inputBufferPosition == 0 && (_byte == '\n' || _byte == '\r' ) )
    {
        /* Skip '\n' and '\r' when the message just started. */
    }
    else
    {
        _parser->m_inputBuffer[_parser->m_inputBufferPosition] = _byte;
        
        if ( _parser->m_inputBuffer[_parser->m_inputBufferPosition] == '\n' || _parser->m_inputBuffer[_parser->m_inputBufferPosition] == '\r' )
        {
           sq_parser_internal_parse_input_buffer ( _parser, _outputStream );
           _parser->m_inputBufferPosition = 0;
        }
        else
        {
           _parser->m_inputBufferPosition ++;
        }
    }
}

void sq_parser_destroy ( SQParser * _parser )
{
   SQ_UNUSED_PARAMETER(_parser);
}
