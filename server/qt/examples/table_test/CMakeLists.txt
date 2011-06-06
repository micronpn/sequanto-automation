ADD_EXECUTABLE(table_test table_test.cxx ${CMAKE_SOURCE_DIR}/server/cxx/src/cxx_root.cpp)

TARGET_LINK_LIBRARIES(table_test ${QT_LIBRARIES} SequantoAutomation_QT)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/server/cxx/src)
