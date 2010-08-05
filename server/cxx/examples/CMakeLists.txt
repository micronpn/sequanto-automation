# Create our output directory for the generated automation files.
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/cxx_example)

IF(SQ_CXX)
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/cxx_example/cxx_example_automation.c GENERATED)
  
  ADD_EXECUTABLE(cxx_example cxx_example.cpp ${CMAKE_BINARY_DIR}/generated/cxx_example/cxx_example_automation.c)
  
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/cxx_example/cxx_example_automation.c
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/cxx/examples/cxx_example.automation
                   COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/cxx/examples/cxx_example.automation
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated/cxx_example
                   VERBATIM)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated/cxx_example)
    
  TARGET_LINK_LIBRARIES(cxx_example SequantoAutomation_CXX)
ENDIF(SQ_CXX)
