IF(TARGET_SUPPORTS_SHARED_LIBRARIES)
  SET(LABVIEW_AUTOMATION_SRCS
      parameters.c)

  SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/applications/labview_automation/labview_automation.c GENERATED)

  ADD_LIBRARY(labview_automation SHARED ${LABVIEW_AUTOMATION_SRCS}
              ${CMAKE_BINARY_DIR}/generated/applications/labview_automation/labview_automation.c)

  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/applications/labview_automation/labview_automation.c
                     MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/applications/labview_automation/labview.automation
                     COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/applications/labview_automation/labview.automation
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated/applications/labview_automation #${CMAKE_BINARY_DIR}/generated
                     VERBATIM)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include ${CMAKE_BINARY_DIR}/generated/applications/labview_automation)

  TARGET_LINK_LIBRARIES(labview_automation SequantoAutomation)
ENDIF(TARGET_SUPPORTS_SHARED_LIBRARIES)
