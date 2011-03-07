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
