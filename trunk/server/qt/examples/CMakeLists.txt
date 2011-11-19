# Create our output directory for the generated automation files.
#FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_example)

IF(SQ_CXX)
  IF(QT4_FOUND)
    FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
    INCLUDE(${QT_USE_FILE})
    
    QT4_WRAP_CPP(qt_automation_MAINWIN_MOC qt_automation/mainwin.h)
    QT4_WRAP_UI(qt_automation_UIS_H qt_automation/mainwin.ui)
    SQ_GENERATE(qt_automation qt_automation/qt_automation.automation)
    ADD_EXECUTABLE(qt_automation_example ${qt_automation_UIS_H} ${qt_automation_MAINWIN_MOC} qt_automation/mainwin.cpp qt_automation/qt_automation.cpp ${SQ_GENERATE_qt_automation})
    
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/qt/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    
    TARGET_LINK_LIBRARIES(qt_automation_example SequantoAutomation_CXX)
    TARGET_LINK_LIBRARIES(qt_automation_example SequantoAutomation_QT)
    TARGET_LINK_LIBRARIES(qt_automation_example ${QT_LIBRARIES})
    
    ADD_SUBDIRECTORY(qt_position_test)
    ADD_SUBDIRECTORY(qt_add_widget_test)
    ADD_SUBDIRECTORY(table_test)
    ADD_SUBDIRECTORY(window_change_test)
  ENDIF(QT4_FOUND)
ENDIF(SQ_CXX)
