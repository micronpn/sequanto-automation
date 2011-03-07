IF(SQ_CXX)
  SET(CXX_SRCS src/tree.cxx
               src/node.cxx
               src/nodeinfo.cxx
               src/methodinfo.cxx
               src/listnode.cxx
               src/propertynode.cxx
               src/readonlypropertynode.cxx
               src/readwritepropertynode.cxx
               src/constantnode.cxx
               src/mutex.cxx
               src/lock.cxx)

  SET(CXX_HEADERS include/sequanto/tree.h
                  include/sequanto/node.h
                  include/sequanto/nodeinfo.h
                  include/sequanto/methodinfo.h
                  include/sequanto/listnode.h
                  include/sequanto/propertynode.h
                  include/sequanto/readonlypropertynode.h
                  include/sequanto/readwritepropertynode.h
                  include/sequanto/constantnode.h
                  include/sequanto/mutex.h
                  include/sequanto/lock.h)
  
  ADD_LIBRARY(SequantoAutomation_CXX STATIC ${CXX_SRCS} ${CXX_HEADERS})
  TARGET_LINK_LIBRARIES(SequantoAutomation_CXX SequantoAutomation)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/c/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/server/cxx/include)

  INSTALL(TARGETS SequantoAutomation_CXX
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)

  INSTALL(FILES ${CXX_HEADERS}
          DESTINATION include/sequanto)
  
  ADD_SUBDIRECTORY(examples)
ENDIF(SQ_CXX)
