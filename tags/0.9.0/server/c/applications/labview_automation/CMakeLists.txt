IF(TARGET_SUPPORTS_SHARED_LIBRARIES)
  SET(LABVIEW_AUTOMATION_SRCS
    parameters.c)

  SQ_GENERATE(labview labview.automation)
  ADD_LIBRARY(labview_automation SHARED ${LABVIEW_AUTOMATION_SRCS}
    ${SQ_GENERATE_labview})
  
  TARGET_LINK_LIBRARIES(labview_automation SequantoAutomation)
ENDIF(TARGET_SUPPORTS_SHARED_LIBRARIES)
