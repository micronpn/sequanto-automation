IF(SQ_ARDUINO)
  SQ_GENERATE(arduino_io arduino_io.automation)
  ADD_EXECUTABLE(arduino_io main.c ${SQ_GENERATE_arduino_io})
  TARGET_LINK_LIBRARIES(arduino_io SequantoAutomation)
  
  SQ_GENERATE_ARDUINO(arduino_io)
    
  FILE(WRITE ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "SET PORT=%1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "if x%1 == x ( SET PORT=COM8 )\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:arduino_io.hex:i\n")
ENDIF(SQ_ARDUINO)
