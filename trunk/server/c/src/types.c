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

#include <sequanto/types.h>

char GetNextFixedLength( SQStringOut *pString )
{
	return (pString->pText)[pString->index++];
}

SQBool HasMoreFixedLength( SQStringOut *pString )
{
	return ( pString->index < pString->length ) ? SQ_TRUE : SQ_FALSE;
}

SQStringOut FixedLengthString(char *pText, int length)
{
	SQStringOut so;
	so.GetNext = GetNextFixedLength;
	so.HasMore = HasMoreFixedLength;
	so.index = 0;
	so.pText = pText;
	so.length = length;
	return so;
}



#ifdef SQ_ARDUINO

#include <avr/pgmspace.h>

static char buffer[100];

const char * const sq_get_constant_string ( const char * const _constantString )
{
   return strcpy_P ( buffer, _constantString );
}

#else

const char * const sq_get_constant_string ( const char * const _constantString )
{
   return _constantString;
}

#endif
