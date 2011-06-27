IF(NOT SQ_ARDUINO)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/machine_automation.c GENERATED)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/machine_automation.c
                     MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/applications/machine_automation/machine.automation
                     COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/applications/machine_automation/machine.automation
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
                     VERBATIM)
                   
ADD_EXECUTABLE(machine_automation main.c files.c memory.c process.c ${CMAKE_BINARY_DIR}/generated/machine_automation.c)

TARGET_LINK_LIBRARIES(machine_automation SequantoAutomation)
TARGET_LINK_LIBRARIES(machine_automation psapi)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)
ENDIF(NOT SQ_ARDUINO)
