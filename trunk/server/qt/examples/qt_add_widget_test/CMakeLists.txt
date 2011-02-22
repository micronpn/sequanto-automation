#QT4_WRAP_CPP(qt_position_test_POSTESTWIN_MOC postestwin.h)
#QT4_WRAP_UI(qt_position_test_UIS_H postestwin.ui)

#ADD_EXECUTABLE(qt_position_test ${qt_position_test_UIS_H} ${qt_position_test_POSTESTWIN_MOC} postestwin.cpp)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_add_widget_test/)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/qt_add_widget_test/add_widget_test_automation.c GENERATED)

ADD_EXECUTABLE(qt_add_widget_test add_widget_test.cpp ${CMAKE_BINARY_DIR}/generated/qt_add_widget_test/add_widget_test_automation.c)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/qt_add_widget_test/add_widget_test_automation.c
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/qt/examples/qt_add_widget_test/add_widget_test.automation
                   COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/qt/examples/qt_add_widget_test/add_widget_test.automation
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_add_widget_test
                   VERBATIM)
                 
TARGET_LINK_LIBRARIES(qt_add_widget_test SequantoAutomation_CXX)
TARGET_LINK_LIBRARIES(qt_add_widget_test SequantoAutomation_QT)
TARGET_LINK_LIBRARIES(qt_add_widget_test ${QT_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
