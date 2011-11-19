IF(NOT SQ_ARDUINO)
  SQ_GENERATE(machine machine.automation)
  ADD_EXECUTABLE(machine_automation main.c files.c memory.c process.c ${SQ_GENERATE_machine})
  TARGET_LINK_LIBRARIES(machine_automation SequantoAutomation)
  
  IF(SQ_WIN32)
    TARGET_LINK_LIBRARIES(machine_automation psapi)
  ENDIF(SQ_WIN32)
ENDIF(NOT SQ_ARDUINO)
