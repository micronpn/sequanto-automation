IF(SQ_ARDUINO)
  SET(ACME_VENTILATOR_SRCS
      main.c
      ventilator.c
      temperature.c
      clock.c
      display.cxx
      ../arduino_demo/LCD03.cpp)

  SQ_GENERATE(acme_ventilator acme_ventilator.automation)

  # INCLUDE_DIRECTORIES(../arduino_demo)

  # ADD_EXECUTABLE(acme_ventilator ${ACME_VENTILATOR_SRCS} ${SQ_GENERATE_acme_ventilator})

  # TARGET_LINK_LIBRARIES(acme_ventilator SequantoAutomation)

#  SQ_GENERATE_ARDUINO(acme_ventilator)
ENDIF(SQ_ARDUINO)
