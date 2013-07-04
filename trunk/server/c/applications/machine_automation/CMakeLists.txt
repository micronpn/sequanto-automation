IF(NOT SQ_ARDUINO)
  SQ_GENERATE(machine machine.automation)
  SET(SOURCES_WITHOUT_GENERATED files.c memory.c process.c server.c)

  IF(SQ_QT_MACHINE_AUTOMATION)
    LIST(APPEND SOURCES_WITHOUT_GENERATED windows_qt.cxx)
    
    QT4_WRAP_CPP(SequantoAutomation_QT_APPLICATION_MACHINE_AUTOMATION_EVENT_FILTER_MOC QtApplicationMachineAutomationEventFilter.h)
    
    LIST(APPEND SOURCES_WITHOUT_GENERATED QtApplicationMachineAutomationEventFilter.cxx)
    LIST(APPEND SOURCES_WITHOUT_GENERATED ${SequantoAutomation_QT_APPLICATION_MACHINE_AUTOMATION_EVENT_FILTER_MOC})
    
    INCLUDE_DIRECTORIES(../../../qt/include)
    
    LIST(APPEND SOURCES_WITHOUT_GENERATED QtMachineAutomationEvent.cxx)
  ENDIF(SQ_QT_MACHINE_AUTOMATION)
  
  IF(SQ_WIN32)  
    LIST(APPEND SOURCES_WITHOUT_GENERATED process_win32.c)
    
    IF(NOT SQ_QT_MACHINE_AUTOMATION)
      LIST(APPEND SOURCES_WITHOUT_GENERATED windows_win32_oleacc.c)
    ENDIF(NOT SQ_QT_MACHINE_AUTOMATION)
  ELSE(SQ_WIN32)
    INCLUDE(FindPkgConfig)

    PKG_CHECK_MODULES(CSPI1 cspi-1.0)
    PKG_CHECK_MODULES(ATSPI2 glib-2.0 atspi-2)
    
    LIST(APPEND SOURCES_WITHOUT_GENERATED process_linux.c)

    IF(NOT SQ_QT_MACHINE_AUTOMATION)
      IF(ATSPI2_FOUND)
        INCLUDE_DIRECTORIES(${ATSPI2_INCLUDE_DIRS})
        LINK_DIRECTORIES(${ATSPI2_LIBRARY_DIRS})
        SET(CFLAGS ${CFLAGS} ${ATSPI2_CFLAGS})
        SET(LDFLAGS ${LDFLAGS} ${ATSPI2_LDFLAGS})
        LIST(APPEND SOURCES_WITHOUT_GENERATED windows_atspi2.c)
      ELSE(ATSPI2_FOUND)
        IF(CSPI1_FOUND)
          INCLUDE_DIRECTORIES(${CSPI1_INCLUDE_DIRS})
          LINK_DIRECTORIES(${CSPI1_LIBRARY_DIRS})
          SET(CFLAGS ${CFLAGS} ${CSPI1_CFLAGS})
          SET(LDFLAGS ${LDFLAGS} ${CSPI1_LDFLAGS})
          LIST(APPEND SOURCES_WITHOUT_GENERATED windows_cspi1.0.c)
        ELSE(CSPI1_FOUND)
          LIST(APPEND SOURCES_WITHOUT_GENERATED windows.c)
        ENDIF(CSPI1_FOUND)
      ENDIF(ATSPI2_FOUND)
    ENDIF(NOT SQ_QT_MACHINE_AUTOMATION)
  ENDIF(SQ_WIN32)

  SET(SOURCES ${SOURCES_WITHOUT_GENERATED} ${SQ_GENERATE_machine})
  
  IF(SQ_QT_MACHINE_AUTOMATION)
    ADD_LIBRARY(machine_automation ${SOURCES})
    IF(NOT QT4_FOUND)
      ERROR("QT4 must be found for the SQ_QT_MACHINE_AUTOMATION flag to be set.")
    ENDIF(NOT QT4_FOUND)
    
    INCLUDE(${QT_USE_FILE})
    
    TARGET_LINK_LIBRARIES(machine_automation ${QT_LIBRARIES})

     IF(SQ_GENERATE_QMAKE)
       STRING(REPLACE ";" " \\\n        src/" SOURCES_REPLACED "src/${SOURCES_WITHOUT_GENERATED}")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "# Machine Automation files\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "MACHINE_AUTOMATION_SRCS=${SOURCES_REPLACED} \\\n        src/machine_automation.c\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "MACHINE_AUTOMATION_HEADERS=src/common.h \\\n        src/machine_automation.h\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "# Putting it all together\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "TEMPLATE=lib\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "SOURCES+=\$\${C_SRCS} \$\${MACHINE_AUTOMATION_SRCS}\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "HEADERS+=\$\${C_HEADERS}\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "INCLUDEPATH+=include\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "win32:LIBS+=WS2_32.lib kernel32.lib user32.lib psapi.lib\n")
       FILE(APPEND ${CMAKE_BINARY_DIR}/machine-automation.pro "DEFINES+=SQ_SEQUANTO_AUTOMATION_SOURCE SQ_SEQUANTO_AUTOMATION_SHARED\n")

       INSTALL(FILES ${CMAKE_BINARY_DIR}/machine-automation.pro
               DESTINATION .)
       INSTALL(FILES ${CMAKE_BINARY_DIR}/custom.pro
               DESTINATION .)
       INSTALL(FILES ${SOURCES} common.h ${SQ_GENERATE_machine_HEADER}
         DESTINATION src)
    ENDIF(SQ_GENERATE_QMAKE)
  ELSE(SQ_QT_MACHINE_AUTOMATION)
    ADD_EXECUTABLE(machine_automation main.c ${SOURCES})
    
    ADD_CUSTOM_TARGET(runserver ./machine_automation DEPENDS machine_automation)
  ENDIF(SQ_QT_MACHINE_AUTOMATION)
  
  TARGET_LINK_LIBRARIES(machine_automation SequantoAutomation)
  
  IF(SQ_WIN32)  
    TARGET_LINK_LIBRARIES(machine_automation psapi)
    TARGET_LINK_LIBRARIES(machine_automation oleacc)
  ELSE(SQ_WIN32)
    IF(ATSPI2_FOUND)
      TARGET_LINK_LIBRARIES(machine_automation ${ATSPI2_LIBRARIES})
    ELSE(ATSPI2_FOUND)
      IF(CSPI1_FOUND)
        TARGET_LINK_LIBRARIES(machine_automation ${CSPI1_LIBRARIES})
      ENDIF(CSPI1_FOUND)
    ENDIF(ATSPI2_FOUND)
  ENDIF(SQ_WIN32)
ENDIF(NOT SQ_ARDUINO)
