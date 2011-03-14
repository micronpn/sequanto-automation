IF(SQ_CXX)
  IF(QT4_FOUND)
     FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
     INCLUDE(${QT_USE_FILE})
     
     QT4_WRAP_CPP(SequantoAutomation_QT_APPLICATION_AUTOMATION_EVENT_FILTER_MOC include/sequanto/QtApplicationAutomationEventFilter.h)
     QT4_WRAP_CPP(SequantoAutomation_QT_WIDGET_AUTOMATION_EVENT_FILTER_MOC include/sequanto/QtWidgetAutomationEventFilter.h)
     QT4_WRAP_CPP(SequantoAutomation_QT_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_MOC include/sequanto/QtPropertyChangedNotificationAdapter.h)
     
     SET(QT_SRCS src/QtWrapper.cxx
                 src/QtWidgetAutomationEventFilter.cxx
                 src/QtApplicationAutomationEventFilter.cxx
                 src/QtWidgetNode.cxx
                 src/QtAutomationGetPropertyEvent.cxx
                 src/QtAutomationMoveEvent.cxx
                 src/QtAutomationResizeEvent.cxx
                 src/QtAutomationMouseMoveEvent.cxx
                 src/QtAutomationMouseClickEvent.cxx
                 src/QtPropertyChangedNotificationAdapter.cxx
                 src/QtUnnamedObjectStore.cxx
                 ${SequantoAutomation_QT_APPLICATION_AUTOMATION_EVENT_FILTER_MOC}
                 ${SequantoAutomation_QT_WIDGET_AUTOMATION_EVENT_FILTER_MOC}
                 ${SequantoAutomation_QT_PROPERTY_CHANGED_NOTIFICATION_ADAPTER_MOC})
     
     SET(QT_HEADERS include/sequanto/QtWrapper.h
                    include/sequanto/QtWidgetAutomationEventFilter.h
                    include/sequanto/QtApplicationAutomationEventFilter.h
                    include/sequanto/QtWidgetNode.h
                    include/sequanto/QtAutomationGetPropertyEvent.h
                    include/sequanto/QtAutomationResizeEvent.h
                    include/sequanto/QtAutomationMoveEvent.h
                    include/sequanto/QtAutomationMouseMoveEvent.h
                    include/sequanto/QtAutomationMouseClickEvent.h
                    include/sequanto/QtPropertyChangedNotificationAdapter.h
                    include/sequanto/QtUnnamedObjectStore.h)
     
     ADD_LIBRARY(SequantoAutomation_QT STATIC ${QT_SRCS} ${QT_HEADERS})
     TARGET_LINK_LIBRARIES(SequantoAutomation_QT SequantoAutomation_CXX)
     TARGET_LINK_LIBRARIES(SequantoAutomation_QT ${QT_LIBRARIES})
    
     INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
     INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
     INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/qt/include)

     INSTALL(TARGETS SequantoAutomation_QT
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)

     INSTALL(FILES ${QT_HEADERS}
             DESTINATION include/sequanto)
           
     ADD_SUBDIRECTORY(examples)
   ENDIF(QT4_FOUND)
ENDIF(SQ_CXX)
