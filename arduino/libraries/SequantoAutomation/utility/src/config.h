/*
 * config.h
 *
 *  Created on: 27/09/2009
 *      Author: Rasmus T. Olesen <rasmus@sequanto.com>
 */

#ifdef SQ_USE_ALTERNATIVE_CONFIG_H
#include "config_alternative.h"
#else

#ifndef _SQ_CONFIG_H_
#define _SQ_CONFIG_H_

#define SQ_SVN_REVISION "462M"
#define CPACK_PACKAGE_VERSION "0.9.0"
#define CPACK_PACKAGE_VENDOR "CIM Software Testing A/S"

#define SQ_USE_BSD_SOCKETS
/* #undef SQ_USE_WINSOCK */
#define SQ_USE_MUTEXES
#define SQ_USE_THREADS
#define HAVE_STDLIB_H
#define HAVE_STDIO_H
#define HAVE_STDINT_H
#define HAVE_SNPRINTF
/* #undef HAVE_UNDERSCORE_SNPRINTF */
/* #undef HAVE_SNPRINTF_S */
/* #undef HAVE_UNDERSCORE_SNPRINTF_S */
/* #undef HAVE_DTOSTRF */
#define HAVE_STRDUP
/* #undef HAVE_UNDERSCORE_STRDUP */
/* #undef HAVE_WINSOCK2_H */
#define HAVE_SYS_SOCKET_H
/* #undef HAVE_INT64_T */
/* #undef SQ_WIN32 */
#define SQ_UNIX
//#define SQ_ARDUINO
#define SQ_LOGGING_ENABLED
#define SQ_C_SUPPORTS_INLINE
#define SQ_USE_ASSERT
/* #undef SQ_BUILD_SHARED_LIBRARIES */

#define SQ_MAX_STRING_LENGTH 1024
#define SQ_MAX_OBJECT_LENGTH 256
#define SQ_MAX_PARAMETERS 10
#define SQ_SOCKET_TIMEOUT 15000
#define SQ_MAX_VALUE_LENGTH 12

#ifdef SQ_LOGGING_ENABLED

#include <stdio.h>

#define SQ_LOG0(format) printf ( format );
#define SQ_LOG1(format, arg1) printf ( format, arg1 );
#define SQ_LOG2(format, arg1, arg2) printf ( format, arg1, arg2 );
#define SQ_LOG3(format, arg1, arg2, arg3) printf ( format, arg1, arg2, arg3 );

#else

#define SQ_LOG0(format)
#define SQ_LOG1(format, arg1)
#define SQ_LOG2(format, arg1, arg2)
#define SQ_LOG3(format, arg1, arg2, arg3)

#endif

#ifdef HAVE_SNPRINTF
#define SNPRINTF_FUNCTION snprintf
#else
#ifdef HAVE_UNDERSCORE_SNPRINTF
#define SNPRINTF_FUNCTION _snprintf
#define HAVE_SNPRINTF
#endif
#endif

#ifdef HAVE_SNPRINTF_S
#define SNPRINTF_S_FUNCTION snprintf_s
#else
#ifdef HAVE_UNDERSCORE_SNPRINTF_S
#define SNPRINTF_S_FUNCTION _snprintf_s
#define HAVE_SNPRINTF_S
#endif
#endif

#ifdef HAVE_UNDERSCORE_STRDUP
#  define SQ_STRDUP_FUNCTION _strdup
#else
#  ifdef HAVE_STRDUP
#    include <string.h>
#    define SQ_STRDUP_FUNCTION strdup
#  else
#    define SQ_STRDUP_FUNCTION sq_internal_strdup
#    ifdef __cplusplus
extern "C"
#    endif
char * sq_internal_strdup ( const char * _value );
#  endif
#endif

#ifdef SQ_C_SUPPORTS_INLINE
#define SQ_INLINE inline
#else
#define SQ_INLINE
#endif

#ifdef SQ_USE_ASSERT
#define SQ_ASSERT assert
#else
#define SQ_ASSERT while(0)
#endif

/* Overrides for Arduino */
#ifdef SQ_ARDUINO
#   if defined(__SAM3X8E__)
#      define SQ_ARDUINO_SAM
#   endif
#undef SQ_USE_MUTEXES
#undef SQ_USE_THREADS
#undef HAVE_SNPRINTF
#ifndef SQ_ARDUINO_SAM
#   define HAVE_DTOSTRF
#endif
#endif

#endif /* _sq_CONFIG_H_ */

#endif /* SQ_USE_ALTERNATIVE_CONFIG_H */
