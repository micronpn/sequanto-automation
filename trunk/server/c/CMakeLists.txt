SET(SRCS src/sq_automation.c src/sq_protocol.c src/sq_server.c
         src/sq_value.c src/sq_system.c src/sq_types.c src/sq_thread.c
         src/sq_mutex.c src/sq_parser.c src/sq_log.c src/sq_ui.c
         src/sq_circularbuffer.c
         ${EXTRA_SRCS})

SET(HEADERS include/sequanto/automation.h include/sequanto/stream.h
            include/sequanto/protocol.h include/sequanto/server.h
            include/sequanto/value.h include/sequanto/system.h
            include/sequanto/log.h include/sequanto/ui.h
            include/sequanto/circularbuffer.h include/sequanto/types.h
            include/sequanto/thread.h include/sequanto/parser.h)

SET(SRCS_UNIT_TEST ${SRCS} src/sq_stream_unit_test.c)

IF(WIN32)
    SET(SQ_WIN32 ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_WINSOCK ON)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    CHECK_INCLUDE_FILE(winsock2.h HAVE_WINSOCK2_H) 

    SET(SRCS ${SRCS} src/sq_stream_tcp.c)
ENDIF(WIN32)

IF(UNIX)
    SET(SQ_UNIX ON)
    SET(SQ_USE_BSD_SOCKETS 1)
    SET(SQ_USE_MUTEXES ON)
    SET(SQ_USE_THREADS ON)
    
    CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H) 
    
    SET(SRCS ${SRCS} src/sq_stream_tcp.c)
ENDIF(UNIX)

IF(SQ_ARDUINO)
    SET(SRCS ${SRCS} src/sq_stream_embedded_serial.c)

    SET(SQ_USE_MUTEXES OFF)
    SET(SQ_USE_THREADS OFF)
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


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)


INSTALL(TARGETS SequantoAutomation
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

INSTALL(FILES ${HEADERS}
        DESTINATION include/sequanto)

CONFIGURE_FILE(src/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(examples)
ADD_SUBDIRECTORY(applications)
