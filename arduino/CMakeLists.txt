IF(SQ_ARDUINO)
    FIND_PACKAGE(Java REQUIRED)
    INCLUDE(UseJava)

    IF(EXISTS "${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
        SET(SQ_ARDUINO_16 FALSE)
        MESSAGE("Using Arduino 1.0.x IDE at ${SQ_ARDUINO_IDE_PATH}")
        MESSAGE("Using ${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
    ELSE()
        IF(EXISTS "${SQ_ARDUINO_IDE_PATH}/lib/arduino-core.jar")
            SET(SQ_ARDUINO_16 TRUE)
            MESSAGE("Using Arduino IDE 1.6 at ${SQ_ARDUINO_IDE_PATH}")
            MESSAGE("Using ${SQ_ARDUINO_IDE_PATH}/lib/arduino-core.jar")
        ELSE()
            MESSAGE(SEND_ERROR "Could not find core.jar at ${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
        ENDIF()
    ENDIF()

    IF(EXISTS "${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
        MESSAGE("Using ${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ELSE()
        MESSAGE(SEND_ERROR "Could not find pde.jar at ${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ENDIF()

    SET(CMAKE_JAVA_COMPILE_FLAGS "-target" "1.5" "-source" "1.5")

    IF(SQ_ARDUINO_16)
        ADD_JAR(SequantoAutomationTool tools/SequantoAutomationTool/src/SequantoAutomationTool.java INCLUDE_JARS "${SQ_ARDUINO_IDE_PATH}/lib/arduino-core.jar" "${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ELSE(SQ_ARDUINO_16)
        ADD_JAR(SequantoAutomationTool tools/SequantoAutomationTool/src/SequantoAutomationTool.java INCLUDE_JARS "${SQ_ARDUINO_IDE_PATH}/lib/core.jar" "${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ENDIF(SQ_ARDUINO_16)

    GET_TARGET_PROPERTY(_jarFile SequantoAutomationTool JAR_FILE)
    GET_TARGET_PROPERTY(_classDir SequantoAutomationTool CLASSDIR)

    MESSAGE(STATUS "Jar file ${_jarFile}")
    MESSAGE(STATUS "Class compiled to ${_classDir}")

    ADD_CUSTOM_COMMAND(TARGET SequantoAutomationTool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${_jarFile}" "${SQ_ARDUINO_OUTPUT}/tools/SequantoAutomationTool/tool/SequantoAutomationTool.jar")

    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libraries DESTINATION "${SQ_ARDUINO_OUTPUT}")

    MACRO(SQ_GENERATE_INO name automation)
        FILE(WRITE "${SQ_ARDUINO_OUTPUT}/libraries/SequantoAutomation/examples/${name}/${name}.ino" " /* BEGIN AUTOMATION\n\n")
        FILE(READ ${automation} CONT)
        FILE(APPEND "${SQ_ARDUINO_OUTPUT}/libraries/SequantoAutomation/examples/${name}/${name}.ino" "${CONT}")
        FILE(APPEND "${SQ_ARDUINO_OUTPUT}/libraries/SequantoAutomation/examples/${name}/${name}.ino" "\n\nEND AUTOMATION */\n\n")
        FOREACH(source ${ARGN})
            FILE(READ ${source} CONT)
            FILE(APPEND "${SQ_ARDUINO_OUTPUT}/libraries/SequantoAutomation/examples/${name}/${name}.ino" "${CONT}")
        ENDFOREACH()
    ENDMACRO()
ENDIF(SQ_ARDUINO)
