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
ENDIF(SQ_ARDUINO)
