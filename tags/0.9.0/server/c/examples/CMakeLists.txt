#
# test_server
#

SQ_GENERATE(test_server test_server.automation)
ADD_EXECUTABLE(test_server test_server.c type_tests.c ${SQ_GENERATE_test_server})

ADD_SUBDIRECTORY(test_server_disabled)

TARGET_LINK_LIBRARIES(test_server SequantoAutomation)
SQ_GENERATE_ARDUINO(test_server)

IF(SQ_ARDUINO)
    #
    # arduino_test
    #
    SQ_GENERATE(arduino_test arduino_test.automation)
    ADD_EXECUTABLE(arduino_test arduino_test.c ${SQ_GENERATE_arduino_test})
    TARGET_LINK_LIBRARIES(arduino_test SequantoAutomation)
    SQ_GENERATE_ARDUINO(arduino_test)
ENDIF(SQ_ARDUINO)

IF(UNIX)
  SQ_GENERATE(liab_test liab_test.automation)
  ADD_EXECUTABLE(liab_test liab_test.c ${SQ_GENERATE_liab_test})
  TARGET_LINK_LIBRARIES(liab_test SequantoAutomation)
ENDIF(UNIX)

