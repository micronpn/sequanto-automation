CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(libSequantoAutomation C CXX)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sequanto Automation library")
SET(CPACK_PACKAGE_VERSION "0.9.1")
SET(CPACK_PACKAGE_VENDOR "CIM Software Testing A/S")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rasmus Toftdahl Olesen <rto@cim.as>")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\code.google.com\\\\sequanto-automation\\\\")

INCLUDE(CPack)

INCLUDE(FindSubversion)
Subversion_WC_INFO(${CMAKE_SOURCE_DIR} SVN)

FIND_PROGRAM(SVNVERSION_COMMAND svnversion)
EXECUTE_PROCESS(COMMAND ${SVNVERSION_COMMAND}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE SQ_SVN_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

INCLUDE(FindPythonInterp)

FIND_PROGRAM(GENERATE_AUTOMATION_DEFINES generate_automation_defines.py
    DOC "Finding generate_automation_defines.py"
    PATHS ${CMAKE_SOURCE_DIR}/generator)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckFunctionExists)

CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(_snprintf HAVE_UNDERSCORE_SNPRINTF)
CHECK_FUNCTION_EXISTS(snprintf_s HAVE_SNPRINTF_S)
CHECK_FUNCTION_EXISTS(_snprintf_s HAVE_UNDERSCORE_SNPRINTF_S)
CHECK_FUNCTION_EXISTS(dtostrf HAVE_DTOSTRF)
CHECK_FUNCTION_EXISTS(strdup HAVE_STRDUP)
CHECK_FUNCTION_EXISTS(_strdup HAVE_UNDERSCORE_STRDUP)

INCLUDE(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(int64_t "stdint.h" HAVE_INT64_T)

IF(CMAKE_CXX_COMPILER)
    INCLUDE(CheckCXXSourceCompiles)
    CHECK_CXX_SOURCE_COMPILES("#include <string>\n int main () { std::string s; return 0; };" STL_SUPPORTED)
ENDIF(CMAKE_CXX_COMPILER)

INCLUDE(CheckCSourceCompiles)
CHECK_C_SOURCE_COMPILES("inline int c() {} int main () { c(); };" SQ_C_SUPPORTS_INLINE)

CHECK_INCLUDE_FILE(assert.h HAVE_ASSERT_H)

IF(STL_SUPPORTED)
    SET(SQ_CXX 1)
ELSE(STL_SUPPORTED)
    SET(SQ_CXX 0)
ENDIF(STL_SUPPORTED)

SET(SQ_QT4   OFF       CACHE BOOL "Build QT4 wrapper functionality")
IF(SQ_QT4)
    INCLUDE(FindQt4)
    FIND_PACKAGE(Qt4)

    SET(SQ_QT_MACHINE_AUTOMATION       OFF       CACHE BOOL "Use the machine automation bridge to automate QT.")
    SET(SQ_GENERATE_QMAKE              OFF       CACHE BOOL "Generate Qmake build files for the machine automation bridge.")
ENDIF(SQ_QT4)

INCLUDE(FindDoxygen)

OPTION(SQ_BUILD_SHARED_LIBRARIES "Build Shared Libraries" OFF)

SET(SQ_MAX_STRING_LENGTH 1024     CACHE STRING "The maximum string length supported by the library")
SET(SQ_MAX_OBJECT_LENGTH 256      CACHE STRING "The maximum object length supported by the library")
SET(SQ_MAX_PARAMETERS    10       CACHE STRING "The maximum number of parameters for function calls")
SET(SQ_SOCKET_TIMEOUT    15000    CACHE STRING "The number of milliseconds to wait for client data before closing the connection")
SET(SQ_LOGGING_ENABLED   ON       CACHE BOOL "Should logging be enabled")
SET(SQ_ARDUINO           OFF      CACHE BOOL "Build arduino 1.0.x plugin.")
IF(SQ_ARDUINO)
    SET(SQ_ARDUINO_IDE_PATH  "SQ_ARDUINO_IDE_PATH-NOT-SPECIFIED"       CACHE PATH "Path to the arduino 1.0.x IDE.")
    SET(SQ_ARDUINO_OUTPUT    "${CMAKE_BINARY_DIR}/SequantoAutomationArduino-${CPACK_PACKAGE_VERSION}"       CACHE PATH "Where to generate the Arduino files.")
ENDIF(SQ_ARDUINO)
SET(SQ_USE_MUTEXES       ON       CACHE BOOL "Use mutexes (Win32 or *nix)")
SET(SQ_USE_THREADS       ON       CACHE BOOL "Use threads (Win32 or *nix)")
SET(SQ_MAX_VALUE_LENGTH  12       CACHE INTEGER "Maximum length of the character buffer used to write integer and float values to a stream.")
IF(HAVE_ASSERT_H)
    SET(SQ_USE_ASSERT        ON       CACHE BOOL "Use assertions to do runtime checks (uses assert macro from assert.h).")
ELSE(HAVE_ASSERT_H)
    SET(SQ_USE_ASSERT        OFF      CACHE BOOL "Use assertions to do runtime checks (uses assert macro from assert.h).")
ENDIF(HAVE_ASSERT_H)


MACRO(SQ_GENERATE automation_name automation_file)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/generated/${automation_name}_automation.c GENERATED)

    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/generated/${automation_name}_automation.c
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${automation_file}
        COMMAND ${PYTHON_EXECUTABLE} ${GENERATE_AUTOMATION_DEFINES} ${CMAKE_CURRENT_SOURCE_DIR}/${automation_file}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
        VERBATIM)

    INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/generated)

    SET(SQ_GENERATE_${automation_name} ${CMAKE_BINARY_DIR}/generated/${automation_name}_automation.c)
    SET(SQ_GENERATE_${automation_name}_HEADER ${CMAKE_BINARY_DIR}/generated/${automation_name}_automation.h)
ENDMACRO(SQ_GENERATE)

IF(DOXYGEN_FOUND)
    SET(SQ_GENERATE_DOCUMENTATION OFF CACHE BOOL "Should documentation be generated (only if doxygen is found)")
ENDIF(DOXYGEN_FOUND)

IF(SQ_GENERATE_DOCUMENTATION)
    IF(NOT DOXYGEN_FOUND)
        MESSAGE(FATAL_ERROR "Doxygen HAS to be found when SQ_GENERATE_DOCUMENTATION is true.")
    ENDIF(NOT DOXYGEN_FOUND)

    SET(DOXY_OUTPUT "${CMAKE_BINARY_DIR}/documentation")

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY )
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/documentation)
    ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)

    FIND_PROGRAM(DOXYGEN2GWIKI_EXECUTABLE doxygen2gwiki
        DOC "Finding doxygen2gwiki (Doxygen to google code wiki converter")

    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/documentation/wiki)

    ADD_CUSTOM_TARGET(gwiki ${PYTHON_EXECUTABLE} ${DOXYGEN2GWIKI_EXECUTABLE} --skip-svn --docs xml --output wiki --prefix API --label C-API --project sequanto-automation --html html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/documentation)
    ADD_DEPENDENCIES(gwiki doc)

    INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/documentation/html/
        DESTINATION docs/api)
ENDIF(SQ_GENERATE_DOCUMENTATION)

IF(SQ_GENERATE_QMAKE)
    SET(SQ_INSTALL_BINARY OFF)
    SET(SQ_INSTALL_SOURCE ON)

    FILE(WRITE ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "# put custom qmake directives in the custom.pro file\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/SequantoAutomation.pro "include(custom.pro)\n\n")
    FILE(APPEND ${CMAKE_BINARY_DIR}/custom.pro "# empty by default\n")

    IF(SQ_QT_MACHINE_AUTOMATION)
        FILE(WRITE ${CMAKE_BINARY_DIR}/machine-automation.pro "# put custom qmake directives in the custom.pro file\n")
        FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "include(custom.pro)\n\n")
    ENDIF(SQ_QT_MACHINE_AUTOMATION)
ELSE(SQ_GENERATE_QMAKE)
    SET(SQ_INSTALL_BINARY ON)
    SET(SQ_INSTALL_SOURCE OFF)
ENDIF(SQ_GENERATE_QMAKE)

ADD_SUBDIRECTORY(arduino)
ADD_SUBDIRECTORY(server)
ADD_SUBDIRECTORY(generator)
