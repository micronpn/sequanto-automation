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

#include <sequanto/types.h>
#include <sequanto/stream.h>
#include <sequanto/value.h>
#include <sequanto/parser.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
 */
void sq_server_init ( SQServer * _server, int _portNumber );

void sq_server_poll ( SQServer * _server );

/**
 * @defgroup Internal Internal functions
 */
/*@{*/

/**
 * The SQServer's callback to handle when data is received on the stream.
 */
void sq_server_handle_stream_data_received ( SQStream * _stream, void * _data, SQByte * _buffer, size_t _length );
/**
 * Pass the input buffer.
 */
void sq_server_parse_input_buffer ( SQServer * _server );

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_SERVER_H_ */
