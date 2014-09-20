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

#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "sequanto/system.h"
#include "sequanto/stream.h"

#include "arduino_serial.h"

typedef struct _SQStream
{
    SQStreamDataReceivedFunction m_handler;
    void * m_handlerData;
} _SQStream;

SQStream * sq_stream_open ( int _portNumber )
{
    SQStream * ret = (SQStream*) malloc ( sizeof(SQStream) );
    ret->m_handler = NULL;
    ret->m_handlerData = NULL;
    return ret;
}

void sq_stream_close ( SQStream * _stream )
{
    free ( _stream );
}

void sq_stream_set_data_received_handler ( SQStream * _stream, SQStreamDataReceivedFunction _handler, void * _data )
{
    _stream->m_handler = _handler;
    _stream->m_handlerData = _data;
}

void sq_stream_poll( SQStream * _stream )
{
}

size_t sq_stream_data_available ( SQStream * _stream )
{
    return arduino_serial_available();
}


SQBool sq_stream_write_string ( SQStream * _stream, const char * const _string )
{
    arduino_serial_write ( _string );
    return SQ_TRUE;
}

SQBool sq_stream_write_SQStringOut ( SQStream * _stream, SQStringOut *pString )
{
    while (pString->HasMore(pString))
    {
        arduino_serial_write_byte( pString->GetNext(pString) );
    }
    return SQ_TRUE;
}

SQBool sq_stream_write_byte ( SQStream * _stream, SQByte _byte )
{
    arduino_serial_write_byte ( _byte );
    return SQ_TRUE;
}

SQBool sq_stream_read_byte ( SQStream * _stream, SQByte * _byte )
{
    int v = arduino_serial_read();
    *_byte = (SQByte) v;
    return SQ_TRUE;
}

void sq_stream_enter_write ( SQStream * _stream )
{
}

void sq_stream_exit_write ( SQStream * _stream )
{
}

void sq_stream_join ( SQStream * _stream )
{
}
