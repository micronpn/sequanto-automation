IF(SQ_ARDUINO)
  SET(
	 ARDUINO_DEMO_SRCS 
	 main.cpp 
	 statemanager.cpp
	 clock.cpp
	 idle.cpp
	 menu.cpp
	 LCD03.cpp
	 tempcontrol.cpp)
  
  SQ_GENERATE(arduino_demo arduino_demo.automation)
  
  # Add the arduino_demo executable, with source files, including the generated one.
  ADD_EXECUTABLE(arduino_demo ${ARDUINO_DEMO_SRCS} ${SQ_GENERATE_arduino_demo})
  
  # Link arduino_demo with the automation library
  TARGET_LINK_LIBRARIES(arduino_demo SequantoAutomation)
  
  SQ_GENERATE_ARDUINO(arduino_demo)
  
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include ${CMAKE_BINARY_DIR}/generated/applications/arduino_demo)
  
  FILE(WRITE ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "SET PORT=%1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "if x%1 == x ( SET PORT=COM8 )\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:server\\c\\applications\\arduino_demo\\arduino_demo.hex:i\n")
ENDIF(SQ_ARDUINO)
