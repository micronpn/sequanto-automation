CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(libSequantoAutomation)

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

SET(SRCS server/c/src/sq_automation.c server/c/src/sq_protocol.c server/c/src/sq_server.c
         server/c/src/sq_value.c server/c/src/sq_system.c server/c/src/sq_types.c server/c/src/sq_thread.c
         server/c/src/sq_mutex.c server/c/src/sq_parser.c server/c/src/sq_log.c server/c/src/sq_ui.c
         server/c/src/sq_circularbuffer.c
         ${EXTRA_SRCS})

SET(HEADERS server/c/include/sequanto/automation.h server/c/include/sequanto/stream.h
            server/c/include/sequanto/protocol.h server/c/include/sequanto/server.h
            server/c/include/sequanto/value.h server/c/include/sequanto/system.h
            server/c/include/sequanto/log.h server/c/include/sequanto/ui.h
            server/c/include/sequanto/circularbuffer.h)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/test_server_automation.c GENERATED)
ADD_EXECUTABLE(test_server server/c/examples/test_server.c server/c/examples/type_tests.c ${CMAKE_BINARY_DIR}/generated/test_server_automation.c)
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/test_server_automation.c
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/test_server.automation
                   COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/test_server.automation
                   WORKING_DIRECTORY generated #${CMAKE_BINARY_DIR}/generated
                   VERBATIM)

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

ADD_SUBDIRECTORY(server/c/tests)
ADD_SUBDIRECTORY(server/c/examples)
ADD_SUBDIRECTORY(server/c/applications)
ADD_SUBDIRECTORY(server/cxx/examples)
ADD_SUBDIRECTORY(server/qt/examples)

SET(SRCS_UNIT_TEST ${SRCS} server/c/src/sq_stream_unit_test.c)

IF(WIN32)
    SET(SQ_WIN32 ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_WINSOCK ON)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    CHECK_INCLUDE_FILE(winsock2.h HAVE_WINSOCK2_H) 

    SET(SRCS ${SRCS} server/c/src/sq_stream_tcp.c)
ENDIF(WIN32)

IF(UNIX)
    SET(SQ_UNIX ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    
    CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H) 
    
    SET(SRCS ${SRCS} server/c/src/sq_stream_tcp.c)
ENDIF(UNIX)

IF(SQ_ARDUINO)
    SET(SRCS ${SRCS} server/c/src/sq_stream_embedded_serial.c)

    SET(SQ_USE_MUTEXES OFF)
    SET(SQ_USE_THREADS OFF)

    ADD_CUSTOM_COMMAND(TARGET test_server
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 test_server test_server.eep
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    ADD_CUSTOM_COMMAND(TARGET test_server
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -R .eeprom test_server test_server.hex
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c GENERATED)

    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/arduino_test.automation
                       COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/arduino_test.automation
                       WORKING_DIRECTORY generated #${CMAKE_BINARY_DIR}/generated
                       VERBATIM)
                       
    ADD_EXECUTABLE(arduino_test server/c/examples/arduino_test.c ${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c)
    
    TARGET_LINK_LIBRARIES(arduino_test SequantoAutomation)
    
    ADD_CUSTOM_COMMAND(TARGET arduino_test                                             
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 arduino_test arduino_test.eep
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    ADD_CUSTOM_COMMAND(TARGET arduino_test
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -R .eeprom arduino_test arduino_test.hex
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)
ENDIF(SQ_ARDUINO)

ADD_LIBRARY(SequantoAutomation STATIC ${SRCS} ${HEADERS})
ADD_LIBRARY(SequantoAutomation_UnitTest STATIC ${SRCS_UNIT_TEST} ${HEADERS})
IF(WIN32)
    TARGET_LINK_LIBRARIES(SequantoAutomation WS2_32)
    TARGET_LINK_LIBRARIES(SequantoAutomation_UnitTest WS2_32)
ENDIF(WIN32)

IF(UNIX)
    TARGET_LINK_LIBRARIES(SequantoAutomation pthread)
    TARGET_LINK_LIBRARIES(SequantoAutomation_UnitTest pthread)
ENDIF(UNIX)

TARGET_LINK_LIBRARIES(test_server SequantoAutomation)

IF(UNIX)
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/liab_test_automation.c GENERATED)
        
        ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/liab_test_automation.c
                           MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/liab_test.automation
                           COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/liab_test.automation
                           WORKING_DIRECTORY generated #${CMAKE_BINARY_DIR}/generated
                           VERBATIM)

        ADD_EXECUTABLE(liab_test server/c/examples/liab_test.c ${CMAKE_BINARY_DIR}/generated/liab_test_automation.c)
        TARGET_LINK_LIBRARIES(liab_test SequantoAutomation)
ENDIF(UNIX)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/server/c/src/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include ${CMAKE_BINARY_DIR}/generated/)

IF(SQ_CXX)
  SET(CXX_SRCS server/cxx/src/tree.cxx
               server/cxx/src/node.cxx
               server/cxx/src/nodeinfo.cxx
               server/cxx/src/methodinfo.cxx
               server/cxx/src/listnode.cxx
               server/cxx/src/propertynode.cxx
               server/cxx/src/readonlypropertynode.cxx
               server/cxx/src/readwritepropertynode.cxx
               server/cxx/src/constantnode.cxx)

  SET(CXX_HEADERS server/cxx/include/sequanto/tree.h
                  server/cxx/include/sequanto/node.h
                  server/cxx/include/sequanto/nodeinfo.h
                  server/cxx/include/sequanto/methodinfo.h
                  server/cxx/include/sequanto/listnode.h
                  server/cxx/include/sequanto/propertynode.h
                  server/cxx/include/sequanto/readonlypropertynode.h
                  server/cxx/include/sequanto/readwritepropertynode.h
                  server/cxx/include/sequanto/constantnode.h)
  
  ADD_LIBRARY(SequantoAutomation_CXX STATIC ${CXX_SRCS} ${CXX_HEADERS})
  TARGET_LINK_LIBRARIES(SequantoAutomation_CXX SequantoAutomation)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)

   IF(QT4_FOUND)
     FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
     INCLUDE(${QT_USE_FILE})

     QT4_WRAP_CPP(SequantoAutomation_QT_AUTOMATION_EVENT_FILTER_MOC server/qt/include/sequanto/qtautomationeventfilter.h)
     
     SET(QT_SRCS server/qt/src/qtwrapper.cxx server/qt/src/qtautomationeventfilter.cxx ${SequantoAutomation_QT_AUTOMATION_EVENT_FILTER_MOC})
     SET(QT_HEADERS server/qt/include/sequanto/qtwrapper.h server/qt/include/sequanto/qtautomationeventfilter.h)
    
     ADD_LIBRARY(SequantoAutomation_QT STATIC ${QT_SRCS} ${QT_HEADERS})
     TARGET_LINK_LIBRARIES(SequantoAutomation_QT SequantoAutomation_CXX)
     TARGET_LINK_LIBRARIES(SequantoAutomation ${QT_LIBRARIES})
    
     INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
     INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/qt/include)
   ENDIF(QT4_FOUND)
ENDIF(SQ_CXX)
