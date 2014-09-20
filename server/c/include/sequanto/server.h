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

#ifndef SEQUANTO_SERVER_H_
#define SEQUANTO_SERVER_H_

#include <sequanto/macros.h>
#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>
#include <sequanto/parser.h>

SQ_BEGIN_DECL

/**
 * @defgroup server Server
 * @ingroup c
 *
 * Contains a target-specific server implementation (serial, tcp,
 * user-defined) ready to serve request from clients connecting to the
 * server.
 *
 * @{
 */

/**
 * The actual server struct, you should never have to access the members inside it directly.
 */
typedef struct _SQServer
{
    SQStream * m_stream;
    int m_clientsHandled;
    SQParser m_parser;
} SQServer;

/**
 * Initialize the SQServer structure, open the connection on the given
 * port and start listening for data.
 *
 * The port number is interpreted differently depending on the
 * platform libSequantoAutomation is running on, if using TCP it will
 * be the port number to listen to. On embedded hardware without TCP
 * support it will normally be the number of the serial port to
 * listen on.
 *
 * This also sets the singleton variable, which means that you can
 * only have one SQServer object at a time.
 */
SQ_DECL void sq_server_init ( SQServer * _server, int _portNumber );

/**
 * Destroy the server, release all resources held by it (connection, memory, etc.).
 */
SQ_DECL void sq_server_destroy ( SQServer * _server );

/**
 * Get the current server singleton instance.
 */
SQ_DECL SQServer * sq_server_get_instance ( void );

/**
 * For TCP targets, this will start listeing on the server socet, and
 * return immediately.
 *
 * For serial targets you need to keep calling this function in your
 * main-loop to keep handling incoming client data.
 */
SQ_DECL void sq_server_poll ( SQServer * _server );

/**
 * The SQServer's callback to handle when data is received on the stream.
 *
 * @ingroup internal
 */

SQ_DECL void sq_server_handle_stream_data_received ( SQStream * _stream, void * _data, SQByte * _buffer, size_t _length );

/**
 * Parse the input buffer.
 *
 * @ingroup internal
 */
SQ_DECL void sq_server_parse_input_buffer ( SQServer * _server );

/**
 * Make the current thead wait for the server to be destroyed by a call to sq_server_destroy.
 *
 * Useful in main() when implementing a server which does nothing but react to requests.
 */
SQ_DECL void sq_server_join ( SQServer * _server );

/**
 * @}
 */

SQ_END_DECL

#endif /* SEQUANTO_SERVER_H_ */
