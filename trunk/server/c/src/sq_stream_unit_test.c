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

#ifdef SQ_USE_ASSERT
#include <assert.h>
#endif

#include "sequanto/system.h"
#include "sequanto/stream.h"
#include "sequanto/circularbuffer.h"

#define BUFFER_SIZE 1024

typedef struct _SQStream
{
    SQStreamDataReceivedFunction m_handler;
    void * m_handlerData;
    SQ_CIRCULAR_BUFFER_DEFINE(m_in, BUFFER_SIZE)
    SQ_CIRCULAR_BUFFER_DEFINE(m_out, BUFFER_SIZE)
} _SQStream;

SQStream * sq_stream_open ( int _portNumber )
{
    SQStream * ret = (SQStream*) malloc ( sizeof(SQStream) );

    SQ_UNUSED_PARAMETER(_portNumber);

    ret->m_handler = NULL;
    ret->m_handlerData = NULL;

    SQ_CIRCULAR_BUFFER_INIT(ret->m_in, BUFFER_SIZE);
    SQ_CIRCULAR_BUFFER_INIT(ret->m_out, BUFFER_SIZE);

    return ret;
}

void sq_stream_close ( SQStream * _stream )
{
    SQ_ASSERT ( _stream != NULL );

    free ( _stream );
}

void sq_stream_set_data_received_handler ( SQStream * _stream, SQStreamDataReceivedFunction _handler, void * _data )
{
    SQ_ASSERT ( _stream != NULL );

    _stream->m_handler = _handler;
    _stream->m_handlerData = _data;
}

void sq_stream_poll( SQStream * _stream )
{
    SQ_ASSERT ( _stream != NULL );
}

size_t sq_stream_data_available ( SQStream * _stream )
{
    SQ_ASSERT ( _stream != NULL );

    return SQ_CIRCULAR_BUFFER_AVAILABLE ( _stream->m_in );
}


SQBool sq_stream_write_string ( SQStream * _stream, const char * const _string )
{
    size_t i;
    size_t length;

    SQ_ASSERT ( _stream != NULL );

    length = strlen(_string);
    for ( i = 0; i < length; i++ )
    {
        sq_stream_write_byte ( _stream, _string[i] );
    }
    return SQ_TRUE;
}

SQBool sq_stream_write_SQStringOut ( SQStream * _stream, SQStringOut *pString )
{
    SQ_ASSERT ( _stream != NULL );

    while (pString->HasMore(pString))
    {
        sq_stream_write_byte( _stream, pString->GetNext(pString) );
    }
    return SQ_TRUE;
}

SQBool sq_stream_write_byte ( SQStream * _stream, SQByte _byte )
{
    SQ_ASSERT ( _stream != NULL );

    SQ_CIRCULAR_BUFFER_PUSH ( _stream->m_out, _byte );
    return SQ_TRUE;
}

SQBool sq_stream_read_byte ( SQStream * _stream, SQByte * _byte )
{
    SQ_ASSERT ( _stream != NULL );

    *_byte = SQ_CIRCULAR_BUFFER_POP ( _stream->m_in );
    return SQ_TRUE;
}

void sq_stream_enter_write ( SQStream * _stream )
{
    SQ_UNUSED_PARAMETER(_stream);
}

void sq_stream_exit_write ( SQStream * _stream )
{
    SQ_UNUSED_PARAMETER(_stream);
}

void sq_stream_join ( SQStream * _stream )
{
    SQ_UNUSED_PARAMETER(_stream);
}

void sq_stream_unit_test_push_read ( SQStream * _stream, SQByte _value )
{
    SQ_ASSERT ( _stream != NULL );

    SQ_CIRCULAR_BUFFER_PUSH(_stream->m_in, _value);
}

SQByte * sq_stream_unit_test_pop_write ( SQStream * _stream )
{
    int i;
    int available;
    SQByte * ret;

    SQ_ASSERT ( _stream != NULL );

    available = SQ_CIRCULAR_BUFFER_AVAILABLE(_stream->m_out);
    ret = malloc ( available + 1 );
    for  ( i = 0; i < available; i++ )
    {
        ret[i] = SQ_CIRCULAR_BUFFER_POP(_stream->m_out);
    }
    ret[available] = 0;
    return ret;
}
