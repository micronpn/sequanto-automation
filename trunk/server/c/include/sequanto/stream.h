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

#ifndef SEQUANTO_STREAM_H_
#define SEQUANTO_STREAM_H_

#include <sequanto/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _SQStream SQStream;

typedef void (*SQStreamDataReceivedFunction) ( SQStream * _stream, void * _data, SQByte * _buffer, size_t _dataLength );

/**
 * Open a stream listening for connections on the given port number
 * (if using TCP) or just listening for data on the serial port with
 * number _portNumber.
 * 
 * Although you may register a callback function using
 * sq_stream_set_data_received_handler it will only be called if
 * sq_stream_poll is called periodically.
 */
SQStream * sq_stream_open ( int _portNumber );

/**
 * Register a function that should be called when data is received
 * from the stream.
 * 
 * @param _stream The stream to subscribe to.
 * @param _handler The handler to be called when data is available.
 * @param _data _data Extra data that will be supplied to the handler when it is invoked.
 */
void sq_stream_set_data_received_handler ( SQStream * _stream, SQStreamDataReceivedFunction _handler, void * _data );

/**
 * Poll the stream for data and, if using TCP, accept any client waiting to be accepted.
 */
void sq_stream_poll( SQStream * _stream );

/**
 * Return the number of bytes available to be read from the stream.
 */
size_t sq_stream_data_available ( SQStream * _stream );

/**
 * Write a single byte to the stream, returns true if the write was
 * successful.
 */
SQBool sq_stream_write_byte ( SQStream * _stream, SQByte _byte );

/**
 * Write a zero terminated string to the stream, returns true if all
 * the characters were written successfully.
 */
SQBool sq_stream_write_string ( SQStream * _stream, const char * const _string );

/**
 * Read a single byte from the stream, returns true if the read was
 * successful.
 */
SQBool sq_stream_read_byte ( SQStream * _stream, SQByte * _byte );

/**
 * To avoid multiple people writing to the stream at once you should
 * call sq_stream_enter_write and sq_stream_exit_write before and
 * after a write operation which might consist of more than one call
 * to the various sq_stream_write_* functions.
 * 
 * @see sq_stream_enter_write
 */
void sq_stream_enter_write ( SQStream * _stream );
/**
 * @see sq_stream_exit_write
 */
void sq_stream_exit_write ( SQStream * _stream );

void sq_stream_close (SQStream * _stream );

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_STREAM_H_ */
