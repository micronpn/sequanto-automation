CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(libSequantoAutomation)

INCLUDE(CPack)

INCLUDE(FindSubversion)
Subversion_WC_INFO(${CMAKE_SOURCE_DIR} SVN)

FIND_PROGRAM(SVNVERSION_COMMAND svnversion)
EXECUTE_PROCESS(COMMAND ${SVNVERSION_COMMAND}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE SVNVERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE)

ADD_DEFINITIONS(-DSVN_REVISION="${SVNVERSION}")

INCLUDE(FindPythonInterp)

FIND_PROGRAM(GENERATE_AUTOMATION_DEFINES generate_automation_defines.py
             DOC "Finding generate_automation_defines.py"
             PATHS ${CMAKE_SOURCE_DIR}/generator)

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

INCLUDE(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(_snprintf HAVE_UNDERSCORE_SNPRINTF)
CHECK_FUNCTION_EXISTS(snprintf_s HAVE_SNPRINTF_S)
CHECK_FUNCTION_EXISTS(_snprintf_s HAVE_UNDERSCORE_SNPRINTF_S)

INCLUDE(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(int64_t "stdint.h" HAVE_INT64_T)

INCLUDE(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("#include <string>\n int main () { std::string s; };" STL_SUPPORTED)

INCLUDE(CheckCSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("inline int c() {} int main () { c(); };" SQ_C_SUPPORTS_INLINE)

CHECK_INCLUDE_FILE(assert.h HAVE_ASSERT_H)

IF(STL_SUPPORTED)
  SET(SQ_CXX 1)
ELSE(STL_SUPPORTED)
  SET(SQ_CXX 0)
ENDIF(STL_SUPPORTED)

SET(SQ_QT4   OFF       CACHE BOOL "Build QT4 wrapper functionality")
IF(SQ_QT4)
  INCLUDE(FindQt4)
  FIND_PACKAGE(Qt4)
ENDIF(SQ_QT4)

INCLUDE(FindDoxygen)

SET(SQ_MAX_STRING_LENGTH 1024     CACHE STRING "The maximum string length supported by the library")
SET(SQ_MAX_OBJECT_LENGTH 256      CACHE STRING "The maximum object length supported by the library")
SET(SQ_MAX_PARAMETERS    10       CACHE STRING "The maximum number of parameters for function calls")
SET(SQ_SOCKET_TIMEOUT    15000    CACHE STRING "The number of milliseconds to wait for client data before closing the connection")
SET(SQ_LOGGING_ENABLED   ON       CACHE BOOL "Should logging be enabled")
SET(SQ_ARDUINO           OFF      CACHE BOOL "Build for arduino")
SET(SQ_USE_MUTEXES       ON       CACHE BOOL "Use mutexes (Win32 or *nix)")
SET(SQ_USE_THREADS       ON       CACHE BOOL "Use threads (Win32 or *nix)")
SET(SQ_MAX_VALUE_LENGTH  12       CACHE INTEGER "Maximum length of the character buffer used to write integer and float values to a stream.")
IF(HAVE_ASSERT_H)
  SET(SQ_USE_ASSERT        ON       CACHE BOOL "Use assertions to do runtime checks (uses assert macro from assert.h).")
ELSE(HAVE_ASSERT_H)
  SET(SQ_USE_ASSERT        OFF      CACHE BOOL "Use assertions to do runtime checks (uses assert macro from assert.h).")
ENDIF(HAVE_ASSERT_H)


IF(DOXYGEN_FOUND)
    SET(SQ_GENERATE_DOCUMENTATION OFF CACHE BOOL "Should documentation be generated (only if doxygen is found)")
ENDIF(DOXYGEN_FOUND)

IF(SQ_GENERATE_DOCUMENTATION)
  SET(DOXY_OUTPUT "${CMAKE_BINARY_DIR}/documentation")
  
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_BINARY_DIR}/Doxyfile
                 @ONLY )
  FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/documentation)
  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
  
  FIND_PROGRAM(DOXYGEN2GWIKI_EXECUTABLE doxygen2gwiki
               DOC "Finding doxygen2gwiki (Doxygen to google code wiki converter")

  FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/documentation/wiki)
  
  ADD_CUSTOM_TARGET(gwiki ${PYTHON_EXECUTABLE} ${DOXYGEN2GWIKI_EXECUTABLE} --skip-svn --docs xml --output wiki --prefix API --label C-API --project sequanto-automation --html html
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/documentation)
  ADD_DEPENDENCIES(gwiki doc)
ENDIF(SQ_GENERATE_DOCUMENTATION)

ADD_SUBDIRECTORY(server/c)
ADD_SUBDIRECTORY(server/cxx)
ADD_SUBDIRECTORY(server/qt)
