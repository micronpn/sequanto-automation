IF(NOT SQ_ARDUINO)
  IF(SQ_WIN32)
    SQ_GENERATE(machine machine.automation)
    SET(SOURCES main.c files.c memory.c process.c ${SQ_GENERATE_machine})
    
    LIST(APPEND SOURCES windows_win32_oleacc.c)
    
    ADD_EXECUTABLE(machine_automation ${SOURCES})
    TARGET_LINK_LIBRARIES(machine_automation SequantoAutomation)
    TARGET_LINK_LIBRARIES(machine_automation psapi)
    TARGET_LINK_LIBRARIES(machine_automation oleacc)
  ENDIF(SQ_WIN32)
ENDIF(NOT SQ_ARDUINO)
