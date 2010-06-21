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

INCLUDE(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(_snprintf HAVE_UNDERSCORE_SNPRINTF)

SET(SRCS server/c/src/automation.c server/c/src/protocol.c server/c/src/server.c
         server/c/src/value.c server/c/src/system.c server/c/src/types.c server/c/src/thread.c
         server/c/src/mutex.c server/c/src/parser.c server/c/src/log.c
         ${EXTRA_SRCS})

SET(HEADERS server/c/include/sequanto/automation.h server/c/include/sequanto/stream.h
            server/c/include/sequanto/protocol.h server/c/include/sequanto/server.h
            server/c/include/sequanto/value.h server/c/include/sequanto/system.h
            server/c/include/sequanto/log.h)

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

ADD_SUBDIRECTORY(server/c/applications)

IF(WIN32)
    SET(SQ_WIN32 ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_WINSOCK ON)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    CHECK_INCLUDE_FILE(winsock2.h HAVE_WINSOCK2_H) 

    SET(SRCS ${SRCS} server/c/src/stream_tcp.c)
ENDIF(WIN32)

IF(UNIX)
    SET(SQ_UNIX ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    
    CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H) 
    
    SET(SRCS ${SRCS} src/stream_tcp.c)
ENDIF(UNIX)

IF(SQ_ARDUINO)
    SET(SRCS ${SRCS} server/c/src/stream_arduino.c)

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
IF(WIN32)
    TARGET_LINK_LIBRARIES(SequantoAutomation WS2_32)
ENDIF(WIN32)

IF(UNIX)
    TARGET_LINK_LIBRARIES(SequantoAutomation pthread)
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
