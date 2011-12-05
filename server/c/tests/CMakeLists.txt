INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(LIB_CHECK check)

CHECK_INCLUDE_FILE(check.h HAVE_CHECK_H)

IF(LIB_CHECK_FOUND AND HAVE_CHECK_H)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include ${LIB_CHECK_INCLUDE_DIRS})
  
  SET(UNIT_TEST_SRCS main.c test_value.c test_parser.c test_values_parse.c test_circularbuffer.c test_server.c test_protocol.c)
  
  ADD_EXECUTABLE(unit_test ${UNIT_TEST_SRCS})

  TARGET_LINK_LIBRARIES(unit_test SequantoAutomation_UnitTest ${LIB_CHECK_LIBRARIES})
ENDIF(LIB_CHECK_FOUND AND HAVE_CHECK_H)
