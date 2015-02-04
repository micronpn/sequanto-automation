IF(SQ_ARDUINO)
  SQ_GENERATE(arduino_io arduino_io.automation)
  # ADD_EXECUTABLE(arduino_io main.c ${SQ_GENERATE_arduino_io})
  # TARGET_LINK_LIBRARIES(arduino_io SequantoAutomation)

#  SQ_GENERATE_ARDUINO(arduino_io main.c ${SQ_GENERATE_arduino_io})
ENDIF(SQ_ARDUINO)
