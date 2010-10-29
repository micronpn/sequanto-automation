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

#include "sequanto/circularbuffer.h"

#include "config.h"

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
#define MAX_UINT 0xFFFFFFFF
#endif

int sq_circular_buffer_available ( int _size, int _readPosition, int _writePosition )
{
    if ( _readPosition == -1 )
    {
        return _size;
    }
    
    if ( _readPosition > _writePosition )
    {
        return _size - _readPosition + _writePosition;
    }
    else
    {
        return _writePosition - _readPosition;
    }
}

SQBool sq_circular_buffer_empty ( int _readPosition, int _writePosition )
{
	if ( _readPosition == _writePosition )
	{
		return SQ_TRUE;
	}
	else
	{
		return SQ_FALSE;
	}
}

SQBool sq_circular_buffer_full ( int _readPosition )
{
    if ( _readPosition == -1 )
    {
        return SQ_TRUE;
    }
    else
    {
        return SQ_FALSE;
    }
}

void sq_circular_buffer_push ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data, SQByte _value )
{
    if ( *_readPosition == -1 )
    {
        *_readPosition = *_writePosition;
    }
    
    _data[*_writePosition] = _value;
    *_writePosition = *_writePosition + 1;
    if ( *_writePosition >= _size )
    {
        *_writePosition = 0;
    }
    
    if ( *_writePosition == *_readPosition )
    {
        *_readPosition = -1;
    }
}

SQByte sq_circular_buffer_pop ( int _size, volatile int * _readPosition, volatile int * _writePosition, SQByte * _data )
{
    if ( *_readPosition == -1 )
    {
        *_readPosition = *_writePosition;
    }
    
    SQByte ret = _data[*_readPosition];
    *_readPosition = *_readPosition + 1;
    if ( *_readPosition >= _size )
    {
        *_readPosition = 0;
    }
    return ret;
}
