# Create our output directory for the generated automation files.
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_example)

IF(SQ_CXX)
  IF(QT4_FOUND)
    FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
    INCLUDE(${QT_USE_FILE})
    
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/qt_example/qt_automation_automation.c GENERATED)

    QT4_WRAP_CPP(qt_automation_MAINWIN_MOC qt_automation/mainwin.h)
    QT4_WRAP_UI(qt_automation_UIS_H qt_automation/mainwin.ui)
    
    ADD_EXECUTABLE(qt_automation_example ${qt_automation_UIS_H} ${qt_automation_MAINWIN_MOC} qt_automation/mainwin.cpp qt_automation/qt_automation.cpp ${CMAKE_BINARY_DIR}/generated/qt_example/qt_automation_automation.c)
    
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/qt_example/qt_automation_automation.c
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/qt/examples/qt_automation/qt_automation.automation
                       COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/qt/examples/qt_automation/qt_automation.automation
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_example
                       VERBATIM)
                     
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/qt/include)
    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated/qt_example)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    
    TARGET_LINK_LIBRARIES(qt_automation_example SequantoAutomation_CXX)
    TARGET_LINK_LIBRARIES(qt_automation_example SequantoAutomation_QT)
    TARGET_LINK_LIBRARIES(qt_automation_example ${QT_LIBRARIES})
    
    ADD_SUBDIRECTORY(qt_position_test)
    ADD_SUBDIRECTORY(qt_add_widget_test)
  ENDIF(QT4_FOUND)
ENDIF(SQ_CXX)
