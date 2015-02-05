#
# test_server
#

SQ_GENERATE(test_server test_server.automation)
ADD_EXECUTABLE(test_server test_server.c type_tests.c ${SQ_GENERATE_test_server})

ADD_SUBDIRECTORY(test_server_disabled)

TARGET_LINK_LIBRARIES(test_server SequantoAutomation)

IF(UNIX)
  SQ_GENERATE(liab_test liab_test.automation)
  ADD_EXECUTABLE(liab_test liab_test.c ${SQ_GENERATE_liab_test})
  TARGET_LINK_LIBRARIES(liab_test SequantoAutomation)
ENDIF(UNIX)

IF(SQ_ARDUINO)
    SQ_GENERATE_INO(test_server test_server.automation test_server.c type_tests.c)
ENDIF(SQ_ARDUINO)
