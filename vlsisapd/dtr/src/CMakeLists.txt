FIND_PACKAGE(Boost 1.32.0 COMPONENTS python)
IF (Boost_FOUND)
  MESSAGE(STATUS "Found Boost.Python libraries in ${Boost_INCLUDE_DIR} as ${Boost_LIBRARIES}")
  FIND_PACKAGE(PythonLibs REQUIRED)
ELSE (Boost_FOUND)
  MESSAGE(STATUS "Boost.Python libraries were not found")
ENDIF (Boost_FOUND)


INCLUDE_DIRECTORIES(${IO_SOURCE_DIR}/dtr/src ${LIBXML2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps io/dtr/Techno.h
           io/dtr/Rules.h
           io/dtr/Name.h
           io/dtr/DTRException.h )

SET ( cpps Techno.cpp
           Name.cpp )

SET ( pycpps PyTechno.cpp ${cpps})

          ADD_LIBRARY(dtr ${cpps})
TARGET_LINK_LIBRARIES(dtr ${LIBXML2_LIBRARIES})
INSTALL(TARGETS dtr     DESTINATION /lib)

IF (Boost_FOUND)
          ADD_LIBRARY(pyDTR MODULE ${pycpps})
SET_TARGET_PROPERTIES(pyDTR PROPERTIES
                            COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                            PREFIX        ""
                     )
TARGET_LINK_LIBRARIES(pyDTR dtr ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
INSTALL(TARGETS pyDTR   DESTINATION /lib/python)
ENDIF(Boost_FOUND)

INSTALL(FILES   ${hpps} DESTINATION /include/io/dtr)
