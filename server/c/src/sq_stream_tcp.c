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

#include <sequanto/system.h>
#include <sequanto/thread.h>

#include "config.h"

#ifndef SQ_USE_THREADS
#error The TCP stream requires threads.
#endif

#ifdef SQ_USE_WINSOCK
#  include <winsock2.h>
#else
#  include <sys/socket.h>
#  include <sys/select.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <unistd.h>
static const ssize_t SOCKET_ERROR = -1;
#endif

#include <sequanto/stream.h>

typedef struct _SQStream
{
    SQStreamDataReceivedFunction m_dataReceivedHandler;
    void * m_dataReceivedHandlerData;
    int m_listenerSocket;
    int m_clientSocket;
    size_t m_dataAvailable;
    SQThread * m_pollingThread;
    SQMutex * m_writeLock;
} _SQStream;

SQStream * sq_stream_open ( int _portNumber )
{
   struct sockaddr_in sa;

   SQStream * ret = malloc ( sizeof(SQStream) );
   ret->m_dataReceivedHandler = NULL;
   ret->m_dataReceivedHandlerData = NULL;

   ret->m_listenerSocket = SOCKET_ERROR;
   ret->m_clientSocket = SOCKET_ERROR;
   ret->m_pollingThread = NULL;
   ret->m_writeLock = sq_mutex_create();
   
   memset ( &sa, 0, sizeof(sa) );
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = INADDR_ANY;
   sa.sin_port = htons(_portNumber);
   ret->m_listenerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef SQ_USE_WINSOCK
   ioctlsocket(ret->m_listenerSocket, FIONBIO, 0 );
#else
   ioctl(ret->m_listenerSocket, FIONBIO, 0 );
#endif
   if ( bind ( ret->m_listenerSocket, (const void*) &sa, sizeof(sa) ) == 0 )
   {
       if ( listen ( ret->m_listenerSocket, 1 ) != 0 )
       {
           SQ_LOG0 ( "Could not listen on port.\n" );
       }
   }
   else
   {
       SQ_LOG0 ( "Could not bind to port.\n" );
   }
   return ret;
}

void sq_stream_internal_close_client ( SQStream * _stream )
{
#  ifdef SQ_USE_WINSOCK
   int ret;
   SQByte byte;
#  endif

   /* Closing any currently open client socket */
    if ( _stream->m_clientSocket != SOCKET_ERROR )
    {
#  ifdef SQ_USE_WINSOCK
        /* According to the Winsock Programmer's FAQ, this is how a socket should be closed.
         * http://tangentsoft.net/wskfaq/newbie.html#howclose
         */
        shutdown ( _stream->m_listenerSocket, 1 );
        ret = recv(_stream->m_listenerSocket, (char*) &byte, 1, 0 );
        while ( ret != 0 && ret != SOCKET_ERROR )
        {
            ret = recv(_stream->m_listenerSocket, (char*) &byte, 1, 0 );
        }
        closesocket ( _stream->m_listenerSocket );
#  else
        close ( _stream->m_listenerSocket );
#endif
    }
    _stream->m_clientSocket = SOCKET_ERROR;
    _stream->m_dataAvailable = 0;
}

void sq_stream_internal_reader ( SQThread * _thread, void * _data )
{
   SQStream * stream = (SQStream*) _data;
   int ret = 0;
   SQByte buff[100];
   do
   {
      ret = recv ( stream->m_clientSocket, buff, 100, 0 );
      if ( ret > 0 && ret != SOCKET_ERROR )
      {
         stream->m_dataReceivedHandler ( stream, stream->m_dataReceivedHandlerData, buff, ret );
      }
   }
   while ( ret != SOCKET_ERROR && ret > 0 );
#ifdef SQ_USE_WINSOCK
   closesocket ( stream->m_clientSocket );
#else
   close ( stream->m_clientSocket );
#endif
   stream->m_clientSocket = SOCKET_ERROR;
}

void sq_stream_internal_polling_thread ( SQThread * _thread, void * _data )
{
    SQThread * clientThread;
    SQStream * stream = (SQStream*) _data;
    int newClient;
    while ( 1 )
    {
        newClient = accept(stream->m_listenerSocket, NULL, NULL );
        if ( stream->m_clientSocket == SOCKET_ERROR && newClient != SOCKET_ERROR )
        {
           stream->m_clientSocket = newClient;
            stream->m_dataAvailable = 0;
            clientThread = sq_thread_create ( sq_stream_internal_reader, stream );
            sq_thread_start ( clientThread );
        }
        else
        {
           if ( newClient != SOCKET_ERROR )
           {
#ifdef SQ_USE_WINSOCK
              closesocket ( newClient );
#else
              close ( newClient );
#endif
           }
        }
    }
}

void sq_stream_poll( SQStream * _stream )
{
   if ( _stream->m_pollingThread == NULL )
   {
       _stream->m_pollingThread = sq_thread_create ( sq_stream_internal_polling_thread, _stream );
       sq_thread_start ( _stream->m_pollingThread );
   }
}

void sq_stream_close ( SQStream * _stream )
{
   sq_stream_internal_close_client ( _stream );
   
#  ifdef SQ_USE_WINSOCK
   closesocket ( _stream->m_listenerSocket );
#  else
   close ( _stream->m_listenerSocket );
#  endif
   sq_mutex_destroy ( _stream->m_writeLock );
   
   free ( _stream );
}

SQBool sq_stream_write_byte ( SQStream * _stream, SQByte _byte )
{
   int sent;
   int now;
   int enter = sq_system_tickcount ();

   do
   {
      sent = send ( _stream->m_clientSocket, (char*) &_byte, 1, 0 );
      
      if ( sent == SOCKET_ERROR )
      {
         return SQ_FALSE;
      }
      if ( sent != 1 )
      {
         now = sq_system_tickcount ();

         if ( now - enter > SQ_SOCKET_TIMEOUT )
         {
            return SQ_FALSE;
         }

         sq_system_sleep ( 100 );
      }
   } while ( sent != 1 );
   return SQ_TRUE;
}

SQBool sq_stream_read_byte ( SQStream * _stream, SQByte * _byte )
{
   int received ;

   received = recv ( _stream->m_clientSocket, (char*) _byte, 1, 0 );
   if ( received == SOCKET_ERROR )
   {
#ifdef SQ_USE_WINSOCK
      SQ_LOG1 ( "SOCKET_ERROR: %i\n", WSAGetLastError() );
#else
      SQ_LOG0 ( "SOCKET_ERROR\n" );
#endif
      return SQ_FALSE;
   }
   if ( received == 1 )
   {
      _stream->m_dataAvailable --;
      return SQ_TRUE;
   }
   else
   {
      return SQ_FALSE;
   }
}

SQBool sq_stream_write_string ( SQStream * _stream, const char * const _string )
{
   size_t i = 0;
   for ( ; _string[i] != '\0'; i++ )
   {
      if ( sq_stream_write_byte ( _stream, _string[i] ) == SQ_FALSE ) return SQ_FALSE;
   }
   return SQ_TRUE;
}

size_t sq_stream_data_available ( SQStream * _stream )
{
   if ( _stream->m_clientSocket != SOCKET_ERROR )
   {
      return _stream->m_dataAvailable;
   }
   else
   {
      return 0;
   }
}

void sq_stream_set_data_received_handler ( SQStream * _stream, SQStreamDataReceivedFunction _handler, void * _data )
{
   _stream->m_dataReceivedHandler = _handler;
   _stream->m_dataReceivedHandlerData = _data;
}

void sq_stream_enter_write ( SQStream * _stream )
{
    sq_mutex_enter ( _stream->m_writeLock );
}

void sq_stream_exit_write ( SQStream * _stream )
{
    sq_mutex_leave ( _stream->m_writeLock );
}
