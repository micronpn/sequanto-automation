IF(SQ_ARDUINO)
  SQ_GENERATE(light_dimmer light_dimmer.automation)

    # ADD_EXECUTABLE(light_dimmer main.cxx DimmerStateMachine.cxx InterfaceStateMachine.cxx display.cxx
    #                  ../arduino_demo/LCD03.cpp
    #                  ${SQ_GENERATE_light_dimmer})

    # TARGET_LINK_LIBRARIES(light_dimmer SequantoAutomation)
    # INCLUDE_DIRECTORIES(../arduino_demo)

#    SQ_GENERATE_ARDUINO(light_dimmer)
ENDIF(SQ_ARDUINO)
