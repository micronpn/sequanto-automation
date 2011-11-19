IF(SQ_ARDUINO)
  SQ_GENERATE(light_dimmer light_dimmer.automation)
                       
    ADD_EXECUTABLE(light_dimmer main.cxx dimmerstatemachine.cxx interfacestatemachine.cxx display.cxx
                     ../arduino_demo/LCD03.cpp
                     ${SQ_GENERATE_light_dimmer})
    
    TARGET_LINK_LIBRARIES(light_dimmer SequantoAutomation)
    INCLUDE_DIRECTORIES(../arduino_demo)
    
    SQ_GENERATE_ARDUINO(light_dimmer)
    
    FILE(WRITE ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "SET PORT=%1\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "if x%1 == x ( SET PORT=COM8 )\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:light_dimmer.hex:i\n")
ENDIF(SQ_ARDUINO)
