SET(WINDOW_CHANGE_SRCS window_change_test.cxx click_handler.h)
QT4_AUTOMOC(${WINDOW_CHANGE_SRCS})
ADD_EXECUTABLE(window_change_test ${WINDOW_CHANGE_SRCS} ${CMAKE_SOURCE_DIR}/server/cxx/src/cxx_root.cpp)
TARGET_LINK_LIBRARIES(window_change_test ${QT_LIBRARIES} SequantoAutomation_QT)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/server/cxx/src)

