IF(SQ_ARDUINO)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/light_dimmer_automation.c GENERATED)

    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/light_dimmer_automation.c
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/server/c/applications/light_dimmer/light_dimmer.automation
                       COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_SOURCE_DIR}/server/c/applications/light_dimmer/light_dimmer.automation
                       WORKING_DIRECTORY generated #${CMAKE_BINARY_DIR}/generated
                       VERBATIM)
                       
    ADD_EXECUTABLE(light_dimmer main.cxx
                     ../arduino_demo/LCD03.cpp
                     ${CMAKE_BINARY_DIR}/generated/light_dimmer_automation.c)
    
    TARGET_LINK_LIBRARIES(light_dimmer SequantoAutomation)
    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated ../arduino_demo)
    
    ADD_CUSTOM_COMMAND(TARGET light_dimmer                                             
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ${CMAKE_BINARY_DIR}/server/c/applications/light_dimmer/light_dimmer light_dimmer.eep
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    ADD_CUSTOM_COMMAND(TARGET light_dimmer
                       POST_BUILD
                       COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_BINARY_DIR}/server/c/applications/light_dimmer/light_dimmer light_dimmer.hex
		               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		               VERBATIM)

    FILE(WRITE ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "SET PORT=%1\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "if x%1 == x ( SET PORT=COM8 )\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/upload_light_dimmer.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:light_dimmer.hex:i\n")
ENDIF(SQ_ARDUINO)
