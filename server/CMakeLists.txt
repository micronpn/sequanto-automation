SET(SQ_ALL_SRCS CACHE INTERNAL "All source files" FORCE)

ADD_SUBDIRECTORY(c)
ADD_SUBDIRECTORY(cxx)
ADD_SUBDIRECTORY(qt)

IF(SQ_BUILD_SHARED_LIBRARIES)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/qt/include)
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)
  
  QT4_WRAP_CPP(SequantoAutomation_SHARED_APPLICATION_AUTOMATION_EVENT_FILTER_MOC qt/include/sequanto/QtApplicationAutomationEventFilter.h)
  QT4_WRAP_CPP(SequantoAutomation_SHARED_WIDGET_AUTOMATION_EVENT_FILTER_MOC qt/include/sequanto/QtWidgetAutomationEventFilter.h)
  QT4_WRAP_CPP(SequantoAutomation_SHARED_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_MOC qt/include/sequanto/QtPropertyChangedNotificationAdapter.h)

  ADD_LIBRARY(SequantoAutomation_SHARED SHARED ${SQ_ALL_SRCS}
                                               ${SequantoAutomation_SHARED_APPLICATION_AUTOMATION_EVENT_FILTER_MOC}
                                               ${SequantoAutomation_SHARED_WIDGET_AUTOMATION_EVENT_FILTER_MOC}
                                               ${SequantoAutomation_SHARED_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_MOC})

  SET_SOURCE_FILES_PROPERTIES(${SQ_ALL_SRCS} PROPERTIES COMPILE_FLAGS
    "-DSQ_SEQUANTO_AUTOMATION_SHARED -DSQ_SEQUANTO_AUTOMATION_SOURCE")
  
  IF(WIN32)
    TARGET_LINK_LIBRARIES(SequantoAutomation_SHARED WS2_32)
  ENDIF(WIN32)
  
  IF(QT4_FOUND)
    INCLUDE(${QT_USE_FILE})
    
    TARGET_LINK_LIBRARIES(SequantoAutomation_SHARED ${QT_LIBRARIES})
  ENDIF(QT4_FOUND)

  ADD_SUBDIRECTORY(shared)
ENDIF(SQ_BUILD_SHARED_LIBRARIES)
