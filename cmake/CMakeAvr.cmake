# this one is important
SET(VERBOSE 1)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCSSOR avr)

# specify the cross compiler
SET(CMAKE_C_COMPILER   avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)
SET(CMAKE_IMPORT_LIBRARY_SUFFIX so)
SET(CMAKE_NO_QUOTED_OBJECTS 1)
SET(CMAKE_C_CREATE_SHARED_LIBRARY "")
SET(CMAKE_C_CREATE_MODULE_LIBRARY "")
SET(CMAKE_C_LINK_EXECUTABLE "")
SET(CMAKE_CXX_LINK_EXECUTABLE "")

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
SET(COPT "-Os")
SET(CXXOPT "-fno-exceptions")
SET(CINCS "-I${CMAKE_SOURCE_DIR}/arduino")
SET(CMCU "-mmcu=atmega328p")
SET(CDEFS "-DF_CPU=16000000L -DSQ_ARDUINO")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA} ${CTUNING}")
#SET(CFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA} ${CTUNING}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT} ${CXXOPT} ${CTUNING}")

ADD_DEFINITIONS(${CFLAGS})

SET (CMAKE_C_FLAGS "")
SET (CMAKE_C_FLAGS_DEBUG "${CFLAGS}")
SET (CMAKE_C_FLAGS_MINSIZEREL "${CFLAGS}")
SET (CMAKE_C_FLAGS_RELEASE "${CFLAGS}")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CFLAGS}")

SET (CMAKE_CXX_FLAGS "")
SET (CMAKE_CXX_FLAGS_DEBUG "${CXXFLAGS}")
SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CXXFLAGS}")
SET (CMAKE_CXX_FLAGS_RELEASE "${CXXFLAGS}")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CXXFLAGS}")

SET(CMAKE_C_FLAGS "${CFLAGS}")
SET(CMAKE_CXX_FLAGS "${CXXFLAGS}")

SET(C_FLAGS "${CFLAGS}")
SET(CXX_FLAGS "${CXXFLAGS}")

SET(SQ_MAX_STRING_LENGTH 256     CACHE STRING "")
SET(SQ_MAX_OBJECT_LENGTH 100      CACHE STRING "")
SET(SQ_MAX_PARAMETERS    5       CACHE STRING "")
SET(SQ_LOGGING_ENABLED   OFF       CACHE BOOL "")
SET(SQ_ARDUINO           ON      CACHE BOOL "")

FILE(WRITE ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "#ifdef __cplusplus\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "extern \"C\" {\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "#endif\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "void arduino_serial_open(long _baudRate);\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "void arduino_serial_write ( const char * const _buff );\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "void arduino_serial_write_byte ( unsigned char _byte );\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "int arduino_serial_read (void);\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "int arduino_serial_available (void);\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "#ifdef __cplusplus\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "}\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.h "#endif\n")

FILE(WRITE ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "#include <HardwareSerial.h>\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "extern \"C\" { \n" )
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "void arduino_serial_open(long _baudRate) { Serial.begin(_baudRate); }\n" )
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "void arduino_serial_write ( const char * const _buff ) { Serial.write ( _buff ); }\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "void arduino_serial_write_byte ( unsigned char _byte ) { Serial.write ( _byte ); }\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "int arduino_serial_read (void) { return Serial.read (); }\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "int arduino_serial_available (void) { return Serial.available (); }\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "void __cxa_pure_virtual(void) {} \n")
FILE(APPEND ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp "}\n")

SET(EXTRA_SRCS ${CMAKE_SOURCE_DIR}/arduino/HardwareSerial.cpp
               ${CMAKE_SOURCE_DIR}/arduino/Print.cpp
               ${CMAKE_SOURCE_DIR}/arduino/wiring.c
               ${CMAKE_SOURCE_DIR}/arduino/wiring_digital.c
               ${CMAKE_SOURCE_DIR}/arduino/wiring_analog.c
               ${CMAKE_SOURCE_DIR}/arduino/wiring_pulse.c
               ${CMAKE_SOURCE_DIR}/arduino/wiring_shift.c
               ${CMAKE_SOURCE_DIR}/arduino/pins_arduino.c
               ${CMAKE_SOURCE_DIR}/arduino/WInterrupts.c
               ${CMAKE_SOURCE_DIR}/arduino/WMath.cpp
               ${CMAKE_SOURCE_DIR}/arduino/Wire.cpp
               ${CMAKE_SOURCE_DIR}/arduino/utility/twi.c
               ${CMAKE_BINARY_DIR}/generated/arduino_serial.cpp)

IF(WIN32)
  FILE(WRITE ${CMAKE_BINARY_DIR}/upload_test_server.bat "SET PORT=%1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_test_server.bat "if x%1 == x ( SET PORT=COM8 )\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_test_server.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:test_server.hex:i\n")
  
  FILE(WRITE ${CMAKE_BINARY_DIR}/upload_arduino_test.bat "SET PORT=%1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_test.bat "if x%1 == x ( SET PORT=COM8 )\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/upload_arduino_test.bat "avrdude -v -cstk500v1 -pm328p -b57600 -P\\\\.\\%PORT% -D -Uflash:w:arduino_test.hex:i\n")
ELSE(WIN32)
  FILE(WRITE ${CMAKE_BINARY_DIR}/generated/upload_test_server.sh "PORT=$1\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/generated/upload_test_server.sh "if [ x$1 == x ]; then PORT=/dev/ttyUSB0; fi\n")
  FILE(APPEND ${CMAKE_BINARY_DIR}/generated/upload_test_server.sh "avrdude -v -cstk500v1 -pm328p -b57600 -P$PORT -D -Uflash:w:test_server.hex:i\n")
  FILE(COPY ${CMAKE_BINARY_DIR}/generated/upload_test_server.sh
            DESTINATION ${CMAKE_BINARY_DIR}
            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
  FILE(REMOVE ${CMAKE_BINARY_DIR}/generated/upload_test_server.sh)
ENDIF(WIN32)
