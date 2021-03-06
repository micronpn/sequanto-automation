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

#include <sequanto/macros.h>
#include <stdlib.h>

SQ_BEGIN_DECL

typedef unsigned char SQByte;
typedef SQByte SQBool;

enum { SQ_TRUE = 1, SQ_FALSE = 0 };

/*
  #define SQ_TRUE 1
  #define SQ_FALSE 0
*/

typedef struct _SQStringOut SQStringOut;

struct _SQStringOut
{
    /* Callback to get the text char by char.*/
    char (*GetNext)(SQStringOut * _string);
    /* Callback that returns true if there are more chars to get by the GetNext callback. */
    SQBool (*HasMore)(SQStringOut * _string);
    union
    {
        void * m_data;
        char * m_text;
    } Data;
    /* Can be used by creator and callbacks to keep track of how many chars that has been returned. */
    int m_index;
    union
    {
        int m_length;
        void * m_extra;
    } Extra;
};

extern char sq_external_get_next_fixed_length( SQStringOut *pString );
extern SQBool sq_external_has_more_fixed_length( SQStringOut *pString );
extern SQStringOut sq_external_fixed_length_string( char *pText, int length );

typedef struct _SQByteArray
{
    SQByte * m_start;
    size_t m_length;
} SQByteArray;

/**
 * Create a byte-array which points to the given data - the data will NOT be copied.
 */
SQ_DECL SQByteArray * sq_byte_array_create ( SQByte * _start, size_t _length );

/**
 * Create a byte-array which points to the given data - the data WILL be copied.
 */
SQ_DECL SQByteArray * sq_byte_array_create_copy ( SQByte * _start, size_t _length );

/**
 * Create a byte-array with pre-allocated space for _length bytes.
 */
SQ_DECL SQByteArray * sq_byte_array_create_prealloc ( size_t _length );

/**
 * Do a full clone of the given byte-array.
 */
SQ_DECL SQByteArray * sq_byte_array_clone ( SQByteArray * _array );

/**
 * Free the given byte-array, including the data if _alsoFreeData is SQ_TRUE.
 */
SQ_DECL void sq_byte_array_free ( SQByteArray * _array, SQBool _alsoFreeData );

#ifdef SQ_ARDUINO
#   include <avr/pgmspace.h>
#   define SQ_STRING_CONSTANT(str) PSTR(str)
#   define SQ_STRING_CONSTANT_FROM_ARRAY(constStr) ((PGM_P) pgm_read_word((constStr)))
#   define SQ_CHARACTER_CONSTANT(str) ((char) pgm_read_byte(str))
#   define SQ_CONST_VARIABLE PROGMEM
#   define SQ_CONSTANT_STRCMP(str, constStr) strcmp_P((PGM_P)(str), (constStr))
#   ifdef SQ_ARDUINO_SAM
#      define SQ_CONSTANT_STRNCMP(str, constStr, len) strncmp((str), (constStr), (len))
#      define SQ_CONSTANT_STRCHR(constStr, c) strchr((constStr),(c))
#   else
#      define SQ_CONSTANT_STRNCMP(str, constStr, len) strncmp_P((str), (constStr), (len))
#      define SQ_CONSTANT_STRCHR(constStr, c) strchr_P((PGM_P)(constStr),(c))
#   endif
#else
#   define SQ_STRING_CONSTANT(str) str
#   define SQ_STRING_CONSTANT_FROM_ARRAY(constStr) *constStr
#   define SQ_CHARACTER_CONSTANT(str) (*(str))
#   define SQ_CONST_VARIABLE
#   define SQ_CONSTANT_STRNCMP(str, constStr, len) strncmp((str), (constStr), (len))
#   define SQ_CONSTANT_STRCMP(str, constStr) strcmp((str), (constStr))
#   define SQ_CONSTANT_STRCHR(constStr, c) strchr((constStr),(c))
#endif

const char * sq_get_constant_string ( const char * const _constantString );
void sq_get_constant_string_copy ( const char * const _constantString, char * _outputBuffer );

SQ_END_DECL

#endif /* SEQUANTO_TYPES_H_ */
