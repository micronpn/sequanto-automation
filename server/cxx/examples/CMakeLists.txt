# Create our output directory for the generated automation files.
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/cxx_example)

IF(SQ_CXX)
  SQ_GENERATE(cxx_example cxx_example.automation)
  
  ADD_EXECUTABLE(cxx_example cxx_example.cpp ${SQ_GENERATE_cxx_example})
  
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
  
  TARGET_LINK_LIBRARIES(cxx_example SequantoAutomation)
  TARGET_LINK_LIBRARIES(cxx_example SequantoAutomation_CXX)
ENDIF(SQ_CXX)
