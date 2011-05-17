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

#ifndef SEQUANTO_CIRCULAR_BUFFER_H_
#define SEQUANTO_CIRCULAR_BUFFER_H_

#include <sequanto/macros.h>
#include <sequanto/types.h>

SQ_BEGIN_DECL

/**
 * @defgroup circularbuffer C implementation of a circular (ring) buffer.
 * @ingroup c
 *
 * Circular buffers are used internally in the sequanto-automation
 * library, but can also be used by user code if desired.
 *
 * Defining and initializing a circular buffer is split up into two
 * parts to allow circular buffers to be defined inside a struct, but
 * initialized later.
 * 
 * @{
 */

/**
 * This function is only used internally, used SQ_CIRCULAR_BUFFER_AVAILABLE(name) instead.
 * 
 * @see SQ_CIRCULAR_BUFFER_AVAILABLE
 * @ingroup internal
 */
SQ_DECL int sq_circular_buffer_available ( int _size, int _readPosition, int _writePosition );
/**
 * This function is only used internally, used SQ_CIRCULAR_BUFFER_EMPTY(name) instead.
 * 
 * @see SQ_CIRCULAR_BUFFER_EMPTY
 * @ingroup internal
 */
SQ_DECL SQBool sq_circular_buffer_empty ( int _readPosition, int _writePosition );
/**
 * This function is only used internally, used SQ_CIRCULAR_BUFFER_FULL(name) instead.
 * 
 * @see SQ_CIRCULAR_BUFFER_FULL
 * @ingroup internal
 */
SQ_DECL SQBool sq_circular_buffer_full ( int _readPosition );
/**
 * This function is only used internally, used SQ_CIRCULAR_BUFFER_PUSH(name, value) instead.
 * 
 * @see SQ_CIRCULAR_BUFFER_PUSH
 * @ingroup internal
 */
SQ_DECL void sq_circular_buffer_push ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data, SQByte _value );
/**
 * This function is only used internally, used SQ_CIRCULAR_BUFFER_POP(name) instead.
 * 
 * @see SQ_CIRCULAR_BUFFER_POP
 * @ingroup internal
 */
SQ_DECL SQByte sq_circular_buffer_pop ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data );

/**
 * Define a circular buffer named parm_name which can hold a maximum of parm_size items.
 * 
 * Remember to call SQ_CURCULAR_BUFFER_INIT on the defined buffer before using it.
 */
#define SQ_CIRCULAR_BUFFER_DEFINE(parm_name, parm_size) struct { int m_size; volatile int m_readPosition; volatile int m_writePosition; SQByte m_data[parm_size]; } parm_name;
/**
 * Initialize the circular buffer defined previously, the parm_size parameter _MUST_ be the same as the one given in SQ_CIRCULAR_BUFFER_DEFINE.
 */
#define SQ_CIRCULAR_BUFFER_INIT(parm_name, parm_size) parm_name.m_size = parm_size; parm_name.m_readPosition = 0; parm_name.m_writePosition = 0;
/** 
 * Returns the number of items available in the buffer.
 */ 
#define SQ_CIRCULAR_BUFFER_AVAILABLE(name) sq_circular_buffer_available ( name.m_size, name.m_readPosition, name.m_writePosition )
/**
 * Returns SQ_TRUE if the circular buffer is empty.
 */
#define SQ_CIRCULAR_BUFFER_EMPTY(name) sq_circular_buffer_empty ( name.m_readPosition, name.m_writePosition )
/**
 * Returns SQ_TRUE if the circular buffer is full.
 */
#define SQ_CIRCULAR_BUFFER_FULL(name) (name.m_readPosition == -1 ? SQ_TRUE : SQ_FALSE)
/**
 * Push value onto the end of the circular buffer.
 */
#define SQ_CIRCULAR_BUFFER_PUSH(name, value) sq_circular_buffer_push ( name.m_size, &name.m_readPosition, &name.m_writePosition, name.m_data, value )
/**
 * Pop the item at the start of the circular buffer.
 */
#define SQ_CIRCULAR_BUFFER_POP(name) sq_circular_buffer_pop ( name.m_size, &name.m_readPosition, &name.m_writePosition, name.m_data )

/**
 * @}
 */
SQ_END_DECL

#endif /* SEQUANTO_CIRCULAR_BUFFER_H_ */
