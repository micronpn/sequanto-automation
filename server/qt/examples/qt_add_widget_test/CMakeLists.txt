IF(SQ_QT_MACHINE_AUTOMATION)
  FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
  INCLUDE(${QT_USE_FILE})
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
  
  ADD_EXECUTABLE(qt_add_widget_test add_widget_test.cpp)
  
  TARGET_LINK_LIBRARIES(qt_add_widget_test machine_automation)
  TARGET_LINK_LIBRARIES(qt_add_widget_test ${QT_LIBRARIES})


ELSE(SQ_QT_MACHINE_AUTOMATION)
  FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/qt_add_widget_test/)

  SQ_GENERATE(add_widget_test add_widget_test.automation)

  ADD_EXECUTABLE(qt_add_widget_test add_widget_test.cpp ${SQ_GENERATE_add_widget_test})

  TARGET_LINK_LIBRARIES(qt_add_widget_test SequantoAutomation_CXX)
  TARGET_LINK_LIBRARIES(qt_add_widget_test SequantoAutomation_QT)
  TARGET_LINK_LIBRARIES(qt_add_widget_test ${QT_LIBRARIES})
  
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ENDIF(SQ_QT_MACHINE_AUTOMATION)
