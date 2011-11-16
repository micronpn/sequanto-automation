#
# test_server
#

SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/test_server_automation.c GENERATED)
ADD_EXECUTABLE(test_server test_server.c type_tests.c ${CMAKE_BINARY_DIR}/generated/test_server_automation.c)
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/test_server_automation.c
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/test_server.automation
                   COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/test_server.automation
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
                   VERBATIM)

ADD_SUBDIRECTORY(test_server_disabled)

TARGET_LINK_LIBRARIES(test_server SequantoAutomation)

SQ_GENERATE_ARDUINO(test_server)

IF(SQ_ARDUINO)
    #
    # arduino_test
    #
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c GENERATED)
    
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/arduino_test.automation
                       COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/arduino_test.automation
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
                       VERBATIM)
                       
    ADD_EXECUTABLE(arduino_test arduino_test.c ${CMAKE_BINARY_DIR}/generated/arduino_test_automation.c)
    
    TARGET_LINK_LIBRARIES(arduino_test SequantoAutomation)

    SQ_GENERATE_ARDUINO(arduino_test)
ENDIF(SQ_ARDUINO)

IF(UNIX)
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/liab_test_automation.c GENERATED)
        
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/liab_test_automation.c
                     MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/examples/liab_test.automation
                     COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/examples/liab_test.automation
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
                     VERBATIM)

  ADD_EXECUTABLE(liab_test liab_test.c ${CMAKE_BINARY_DIR}/generated/liab_test_automation.c)
  TARGET_LINK_LIBRARIES(liab_test SequantoAutomation)
ENDIF(UNIX)

