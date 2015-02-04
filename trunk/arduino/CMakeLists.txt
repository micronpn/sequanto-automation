IF(SQ_ARDUINO)
    FIND_PACKAGE(Java REQUIRED)
    INCLUDE(UseJava)

    MESSAGE("Using Arduino 1.0.x IDE at ${SQ_ARDUINO_IDE_PATH}")

    IF(EXISTS "${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
        MESSAGE("Using ${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
    ELSE()
        ERROR("Could not find core.jar at ${SQ_ARDUINO_IDE_PATH}/lib/core.jar")
    ENDIF()

    IF(EXISTS "${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
        MESSAGE("Using ${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ELSE()
        ERROR("Could not find pde.jar at ${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")
    ENDIF()

    SET(CMAKE_JAVA_COMPILE_FLAGS "-target" "1.7")

    ADD_JAR(SequantoAutomationTool tools/SequantoAutomationTool/src/SequantoAutomationTool.java INCLUDE_JARS "${SQ_ARDUINO_IDE_PATH}/lib/core.jar" "${SQ_ARDUINO_IDE_PATH}/lib/pde.jar")

    GET_TARGET_PROPERTY(_jarFile SequantoAutomationTool JAR_FILE)
    GET_TARGET_PROPERTY(_classDir SequantoAutomationTool CLASSDIR)

    MESSAGE(STATUS "Jar file ${_jarFile}")
    MESSAGE(STATUS "Class compiled to ${_classDir}")

    FILE(COPY ${_jarFile} DESTINATION "${SQ_ARDUINO_OUTPUT}/tools/SequantoAutomationTool/tool")
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libraries DESTINATION "${SQ_ARDUINO_OUTPUT}")
ENDIF(SQ_ARDUINO)
