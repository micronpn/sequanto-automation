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

#include <sequanto/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int sq_circular_buffer_available ( int _size, int _readPosition, int _writePosition );
	 SQBool sq_circular_buffer_empty ( int _readPosition, int _writePosition );
    SQBool sq_circular_buffer_full ( int _readPosition );
    void sq_circular_buffer_push ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data, SQByte _value );
    SQByte sq_circular_buffer_pop ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data );

#define SQ_CIRCULAR_BUFFER_DEFINE(parm_name, parm_size) struct { int m_size; volatile int m_readPosition; volatile int m_writePosition; SQByte m_data[parm_size]; } parm_name;
#define SQ_CIRCULAR_BUFFER_INIT(parm_name, parm_size) parm_name.m_size = parm_size; parm_name.m_readPosition = 0; parm_name.m_writePosition = 0;
#define SQ_CIRCULAR_BUFFER_AVAILABLE(name) sq_circular_buffer_available ( name.m_size, name.m_readPosition, name.m_writePosition )
#define SQ_CIRCULAR_BUFFER_EMPTY(name) sq_circular_buffer_empty ( name.m_readPosition, name.m_writePosition )
#define SQ_CIRCULAR_BUFFER_FULL(name) (name.m_readPosition == -1 ? SQ_TRUE : SQ_FALSE)
#define SQ_CIRCULAR_BUFFER_PUSH(name, value) sq_circular_buffer_push ( name.m_size, &name.m_readPosition, &name.m_writePosition, name.m_data, value )
#define SQ_CIRCULAR_BUFFER_POP(name) sq_circular_buffer_pop ( name.m_size, &name.m_readPosition, &name.m_writePosition, name.m_data )

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_CIRCULAR_BUFFER_H_ */
