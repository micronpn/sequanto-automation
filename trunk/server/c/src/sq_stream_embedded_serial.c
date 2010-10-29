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

#include <avr/interrupt.h>
#include <Wiring.h>

#include "config.h"

#include "sequanto/system.h"
#include "sequanto/stream.h"
#include "sequanto/circularbuffer.h"

#define INTERNAL_BUFFER 10

typedef struct _SQStream
{
    SQStreamDataReceivedFunction m_handler;
    void * m_handlerData;
    SQ_CIRCULAR_BUFFER_DEFINE(m_in, INTERNAL_BUFFER);
    SQ_CIRCULAR_BUFFER_DEFINE(m_out, INTERNAL_BUFFER);
} _SQStream;

#ifndef BAUD_TOL
#   define BAUD_TOL 2
#endif

static SQStream * s_embeddedSerialStream = NULL;

SQStream * sq_stream_open ( int _portNumber )
{
   SQStream * ret = (SQStream*) malloc ( sizeof(SQStream) );
   ret->m_handler = NULL;
   ret->m_handlerData = NULL;
   SQ_CIRCULAR_BUFFER_INIT(ret->m_in, INTERNAL_BUFFER);
   SQ_CIRCULAR_BUFFER_INIT(ret->m_out, INTERNAL_BUFFER);
   
   unsigned long baud = 57600;
   //unsigned long baud = 9600;
   
   uint8_t use2x = 0;
   uint16_t ubbr =  (F_CPU + 8UL * baud) / (16UL * baud) - 1UL;
   if ( (100 * (F_CPU)) > (16 * (ubbr + 1) * (100 * ubbr + ubbr * BAUD_TOL)) ) {
       use2x = 1;
       ubbr = (F_CPU + 4UL * baud) / (8UL * baud) - 1UL;
   }

   UBRR0L = ubbr & 0xff;
   UBRR0H = ubbr >> 8;
   if (use2x) {
       UCSR0A |= (1 << U2X0);
   } else {
       UCSR0A &= ~(1 << U2X0);
   }

   UCSR0B |= (1<<TXEN0);  // Enable Transmitter
   UCSR0B |= (1<<RXEN0);  // Enable Reciever
   UCSR0B |= (1<<RXCIE0); // Enable Rx Complete Interrupt
   
   s_embeddedSerialStream = ret;
   
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
    return SQ_CIRCULAR_BUFFER_AVAILABLE(_stream->m_in);
}


SQBool sq_stream_write_string ( SQStream * _stream, const char * const _string )
{
    const char * p;
    for ( p = _string; *p != 0; p++ )
    {
        sq_stream_write_byte ( _stream, *p );
    }
    return SQ_TRUE;
}

SQBool sq_stream_write_SQStringOut ( SQStream * _stream, SQStringOut *pString )
{
	while (pString->HasMore(pString))
	{
		sq_stream_write_byte ( _stream, pString->GetNext(pString) );
	}
	return SQ_TRUE;
}

SQBool sq_stream_write_byte ( SQStream * _stream, SQByte _byte )
{
    noInterrupts();
	 if (SQ_CIRCULAR_BUFFER_EMPTY(_stream->m_out) == SQ_TRUE)
	 {
	 	if ( UCSR0A & (1<<UDRE0) )
	 	{
			UDR0 = _byte;
	 	}
		else
		{
			SQ_CIRCULAR_BUFFER_PUSH ( _stream->m_out, _byte );
			UCSR0B |= (1<<UDRIE0); // Enable Data Register Empty interrupt. Note: Needed because the UDRIEn is level-triggered, not edge-triggered.
		}
		interrupts();
		return SQ_TRUE;
	 }
	 else
	 {
		 interrupts();	// Allow TX-interrupt to make space in buffer.
	    while ( SQ_CIRCULAR_BUFFER_FULL(_stream->m_out) == SQ_TRUE );
		 SQ_CIRCULAR_BUFFER_PUSH ( _stream->m_out, _byte );
	 }

    return SQ_TRUE;
}

SQBool sq_stream_read_byte ( SQStream * _stream, SQByte * _byte )
{
    if (SQ_CIRCULAR_BUFFER_AVAILABLE(_stream->m_in) == 0 )
    {
        return SQ_FALSE;
    } else {
        *_byte = SQ_CIRCULAR_BUFFER_POP(_stream->m_in);
    }
    return SQ_TRUE;
}

ISR(USART_RX_vect)
{
    uint8_t data = UDR0;
    SQ_CIRCULAR_BUFFER_PUSH ( s_embeddedSerialStream->m_in,  data );
}

/**
 * Data Register Empty Handler
 */
ISR(USART_UDRE_vect)
{
	if ( SQ_CIRCULAR_BUFFER_AVAILABLE(s_embeddedSerialStream->m_out) != 0 )
	{
		UDR0 = SQ_CIRCULAR_BUFFER_POP(s_embeddedSerialStream->m_out);
	}
	else
	{
		// Buffer is empty, disable the interrupt
		UCSR0B &= ~(1<<UDRIE0);
	}
}

void sq_stream_enter_write ( SQStream * _stream )
{
}

void sq_stream_exit_write ( SQStream * _stream )
{
}
