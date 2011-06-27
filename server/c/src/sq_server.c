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

#include "sequanto/server.h"
#include "sequanto/stream.h"
#include "sequanto/protocol.h"
#include "sequanto/value.h"
#include "sequanto/types.h"
#include "sequanto/thread.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

static SQServer * server_instance;

void sq_server_init ( SQServer * _server, int _portNumber )
{
    assert ( _server != NULL );
    
    _server->m_clientsHandled = 0;
    _server->m_stream = sq_stream_open ( _portNumber );
    sq_stream_set_data_received_handler ( _server->m_stream, sq_server_handle_stream_data_received, _server );
    
    server_instance = _server;
    
    sq_parser_init ( &_server->m_parser );
}

void sq_server_destroy ( SQServer * _server )
{
    assert ( _server != NULL );

    sq_stream_close ( _server->m_stream );
    _server->m_stream = NULL;
    
    server_instance = NULL;
}

SQServer * sq_server_get_instance ( void )
{
   return server_instance;
}

void sq_server_internal_handle_byte ( SQServer * _server, SQByte _byte )
{
    assert ( _server != NULL );
    
    sq_parser_input_byte ( &_server->m_parser, _server->m_stream, _byte );
}

void sq_server_poll ( SQServer * _server )
{
   SQByte byte;

   assert ( _server != NULL );
   
   sq_stream_poll ( _server->m_stream );

   if ( !sq_thread_is_supported() )
   {
      while ( sq_stream_data_available(_server->m_stream) > 0 )
      {
         if ( sq_stream_read_byte ( _server->m_stream, &byte ) == SQ_FALSE )
         {
             /* sq_protocol_write_failure_with_text ( _server->m_stream, sq_get_constant_string(SQ_STRING_CONSTANT("sq_server_poll: read_byte returned FALSE\r\n")) ); */
            return;
         }
         sq_server_internal_handle_byte ( _server, byte );
      }
   }
}

void sq_server_handle_stream_data_received ( SQStream * _stream, void * _data, SQByte * _buffer, size_t _length )
{
   size_t i ;
   SQServer * server = (SQServer *) _data;

   SQ_UNUSED_PARAMETER(_stream);
   
   for ( i= 0; i < _length; i++ )
   {
      sq_server_internal_handle_byte ( server, _buffer[i] );
   }
}

void sq_server_join ( SQServer * _server )
{
   sq_stream_join ( _server->m_stream );
}

/*
void sq_server_start( SQServer * _server)
{
SQStream * client;

   while ( _server->m_keepListening == SQ_TRUE )
   {
      client = sq_stream_accept ( _server->m_listener );
      
      _server->m_clientsHandled ++;

      sq_server_handle_client ( _server, client );

      sq_stream_close ( client );
   }
}
*/
