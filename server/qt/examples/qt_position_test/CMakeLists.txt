QT4_WRAP_CPP(qt_position_test_POSTESTWIN_MOC postestwin.h)
QT4_WRAP_UI(qt_position_test_UIS_H postestwin.ui)

ADD_EXECUTABLE(qt_position_test ${qt_position_test_UIS_H} ${qt_position_test_POSTESTWIN_MOC} postestwin.cpp ${CMAKE_SOURCE_DIR}/server/cxx/src/cxx_root.cpp)

TARGET_LINK_LIBRARIES(qt_position_test ${QT_LIBRARIES} SequantoAutomation_QT)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/server/cxx/src)
