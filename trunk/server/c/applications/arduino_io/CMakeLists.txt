IF(SQ_ARDUINO)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/arduino_io_automation.c GENERATED)

    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/arduino_io_automation.c
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/applications/arduino_io/arduino_io.automation
                       COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/applications/arduino_io/arduino_io.automation
                       WORKING_DIRECTORY generated #${CMAKE_BINARY_DIR}/generated
                       VERBATIM)
                       
    ADD_EXECUTABLE(arduino_io main.c ${CMAKE_BINARY_DIR}/generated/arduino_io_automation.c)
    
    TARGET_LINK_LIBRARIES(arduino_io SequantoAutomation)
    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)
    
    ADD_CUSTOM_COMMAND(TARGET arduino_io                                             
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ${CMAKE_BINARY_DIR}/server/c/applications/arduino_io/arduino_io arduino_io.eep
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    ADD_CUSTOM_COMMAND(TARGET arduino_io
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_BINARY_DIR}/server/c/applications/arduino_io/arduino_io arduino_io.hex
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    FILE(WRITE ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "SET PORT=%1\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "if x%1 == x ( SET PORT=COM8 )\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_io.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:arduino_io.hex:i\n")
ENDIF(SQ_ARDUINO)
