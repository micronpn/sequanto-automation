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

  # Create our output directory for the generated automation files.
  FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/applications/arduino_demo)

  # Add the generated automation .c file
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/applications/arduino_demo/arduino_demo_automation.c GENERATED)
  
  # Add the arduino_demo executable, with source files, including the generated one.
  ADD_EXECUTABLE(arduino_demo ${ARDUINO_DEMO_SRCS}
    ${CMAKE_BINARY_DIR}/generated/applications/arduino_demo/arduino_demo_automation.c)

  # Add custom pre-build command to generate the automation .c file from the .automation file.
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/applications/arduino_demo/arduino_demo_automation.c
    MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/applications/arduino_demo/arduino_demo.automation
    COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/applications/arduino_demo/arduino_demo.automation
    WORKING_DIRECTORY generated/applications/arduino_demo #${CMAKE_BINARY_DIR}/generated
    VERBATIM)

  # Link arduino_demo with the automation library
  TARGET_LINK_LIBRARIES(arduino_demo SequantoAutomation)

  # Create the upload-able .hex images.
  ADD_CUSTOM_COMMAND(TARGET arduino_demo
    POST_BUILD
    COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 arduino_demo arduino_demo.eep
	 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	 VERBATIM)

  ADD_CUSTOM_COMMAND(TARGET arduino_demo
    POST_BUILD
    COMMAND avr-objcopy -O ihex -R .eeprom arduino_demo arduino_demo.hex
	 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	 VERBATIM)
  
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include ${CMAKE_BINARY_DIR}/generated/applications/arduino_demo)
  
  FILE(WRITE ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "SET PORT=%1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "if x%1 == x ( SET PORT=COM8 )\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_demo.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:server\\c\\applications\\arduino_demo.hex:i\n")
ENDIF(SQ_ARDUINO)
