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

#ifndef SEQUANTO_TYPES_H_
#define SEQUANTO_TYPES_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned char SQByte;
typedef SQByte SQBool;
static const SQBool SQ_TRUE = 1;
static const SQBool SQ_FALSE = 0;

typedef struct _SQStringOut SQStringOut;

struct _SQStringOut
{
  char (*GetNext)(SQStringOut *pString);  // Callback to get the text char by char.
  SQBool (*HasMore)(SQStringOut *pString);  // Callback that returns true if there are more chars to get by the GetNext callback.
	union
	{
		void *pData;
		char *pText;
	};
	int index;  // Can be used by creator and callbacks to keep track of how many chars that has been returned.
	union
	{
		int length;
		void *pExtra;
	};
};

extern char GetNextFixedLength( SQStringOut *pString );
extern SQBool HasMoreFixedLength( SQStringOut *pString );
extern SQStringOut FixedLengthString( char *pText, int length );



#ifdef SQ_ARDUINO
#   include <avr/pgmspace.h>
#   define SQ_STRING_CONSTANT(str) PSTR(str)
#   define SQ_STRING_CONSTANT_FROM_ARRAY(constStr) ((PGM_P) pgm_read_word((constStr)))
#   define SQ_CHARACTER_CONSTANT(str) ((char) pgm_read_byte(str))
#   define SQ_CONST_VARIABLE PROGMEM
#   define SQ_CONSTANT_STRNCMP(str, constStr, len) strncmp_P((str), (constStr), (len))
#   define SQ_CONSTANT_STRCMP(str, constStr) strcmp_P((PGM_P)(str), (constStr))
#   define SQ_CONSTANT_STRCHR(constStr, c) strchr_P((PGM_P)(constStr),(c))
#else
#   define SQ_STRING_CONSTANT(str) str
#   define SQ_STRING_CONSTANT_FROM_ARRAY(constStr) *constStr
#   define SQ_CHARACTER_CONSTANT(str) (*(str))
#   define SQ_CONST_VARIABLE
#   define SQ_CONSTANT_STRNCMP(str, constStr, len) strncmp((str), (constStr), (len))
#   define SQ_CONSTANT_STRCMP(str, constStr) strcmp((str), (constStr))
#   define SQ_CONSTANT_STRCHR(constStr, c) strchr((constStr),(c))
#endif

const char * const sq_get_constant_string ( const char * const _constantString );

#ifdef __cplusplus
}
#endif

#endif /* SEQUANTO_TYPES_H_ */
