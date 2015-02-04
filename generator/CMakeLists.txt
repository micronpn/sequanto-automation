INSTALL(PROGRAMS generate_automation_defines.py
        DESTINATION generator)

INSTALL(DIRECTORY code
        DESTINATION generator
        FILES_MATCHING PATTERN "*.c"
        PATTERN ".svn" EXCLUDE)


INSTALL(DIRECTORY lib
        DESTINATION generator
        FILES_MATCHING PATTERN "*.py"
        PATTERN ".svn" EXCLUDE)

IF(SQ_ARDUINO)
    FILE(COPY generate_automation_defines.py DESTINATION "${SQ_ARDUINO_OUTPUT}/tools/SequantoAutomationTool/generator")
    FILE(COPY code DESTINATION "${SQ_ARDUINO_OUTPUT}/tools/SequantoAutomationTool/generator")
    FILE(COPY lib DESTINATION "${SQ_ARDUINO_OUTPUT}/tools/SequantoAutomationTool/generator")
ENDIF(SQ_ARDUINO)
