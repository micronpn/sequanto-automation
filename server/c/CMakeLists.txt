SET(C_SRCS src/sq_automation.c src/sq_protocol.c src/sq_server.c
           src/sq_value.c src/sq_system.c src/sq_types.c src/sq_thread.c
           src/sq_mutex.c src/sq_parser.c src/sq_log.c src/sq_ui.c
           src/sq_circularbuffer.c
           ${EXTRA_SRCS})

SET(C_HEADERS include/sequanto/automation.h include/sequanto/stream.h
              include/sequanto/protocol.h include/sequanto/server.h
              include/sequanto/value.h include/sequanto/system.h
              include/sequanto/log.h include/sequanto/ui.h
              include/sequanto/circularbuffer.h include/sequanto/types.h
              include/sequanto/thread.h include/sequanto/parser.h
              include/sequanto/macros.h)

SET(C_SRCS_UNIT_TEST ${C_SRCS} src/sq_stream_unit_test.c)

IF(WIN32)
    SET(SQ_WIN32 ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_WINSOCK ON)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    CHECK_INCLUDE_FILE(winsock2.h HAVE_WINSOCK2_H) 

    SET(C_SRCS ${C_SRCS} src/sq_stream_tcp.c)
ENDIF(WIN32)

IF(UNIX)
    SET(SQ_UNIX ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    
    CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H) 
    
    SET(C_SRCS ${C_SRCS} src/sq_stream_tcp.c)
ENDIF(UNIX)

IF(SQ_ARDUINO)
  SET(C_SRCS ${C_SRCS} src/sq_stream_embedded_serial.c)
  #SET(C_SRCS ${C_SRCS} src/sq_stream_arduino.c)

    SET(SQ_USE_MUTEXES OFF)
    SET(SQ_USE_THREADS OFF)
ENDIF(SQ_ARDUINO)

STRING(REPLACE ";" ";c/" C_SRCS_ALL "${C_SRCS}")
SET(SQ_ALL_SRCS ${SQ_ALL_SRCS} "c/${C_SRCS_ALL}" CACHE INTERNAL "All source files")

ADD_LIBRARY(SequantoAutomation STATIC ${C_SRCS} ${C_HEADERS})
ADD_LIBRARY(SequantoAutomation_UnitTest STATIC ${C_SRCS_UNIT_TEST} ${C_HEADERS})
IF(WIN32)
    TARGET_LINK_LIBRARIES(SequantoAutomation WS2_32)
    TARGET_LINK_LIBRARIES(SequantoAutomation_UnitTest WS2_32)
ENDIF(WIN32)

IF(UNIX)
    TARGET_LINK_LIBRARIES(SequantoAutomation pthread)
    TARGET_LINK_LIBRARIES(SequantoAutomation_UnitTest pthread)
ENDIF(UNIX)


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)

IF(SQ_INSTALL_BINARY)
  INSTALL(TARGETS SequantoAutomation
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)

ENDIF(SQ_INSTALL_BINARY)

INSTALL(FILES ${C_HEADERS}
        DESTINATION include/sequanto)

CONFIGURE_FILE(src/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)

IF(SQ_INSTALL_SOURCE)
  INSTALL(FILES ${C_SRCS}
          DESTINATION src)
  INSTALL(FILES ${CMAKE_BINARY_DIR}/generated/config.h
          DESTINATION src)
ENDIF(SQ_INSTALL_SOURCE)

IF(SQ_GENERATE_QMAKE)
  STRING(REPLACE ";" " \\\n       " C_SRCS_REPLACED "${C_SRCS}")
  STRING(REPLACE ";" " \\\n          " C_HEADERS_REPLACED "${C_HEADERS}")
  FILE(APPEND ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "# C files\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "C_SRCS=${C_SRCS_REPLACED}\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "C_HEADERS=${C_HEADERS_REPLACED}\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "\n")
ENDIF(SQ_GENERATE_QMAKE)

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(examples)
ADD_SUBDIRECTORY(applications)
