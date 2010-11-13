QT4_WRAP_CPP(qt_position_test_POSTESTWIN_MOC postestwin.h)
QT4_WRAP_UI(qt_position_test_UIS_H postestwin.ui)

ADD_EXECUTABLE(qt_position_test ${qt_position_test_UIS_H} ${qt_position_test_POSTESTWIN_MOC} postestwin.cpp)

TARGET_LINK_LIBRARIES(qt_position_test ${QT_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
