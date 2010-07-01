INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/cif/src ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/cif/Circuit.h   
           vlsisapd/cif/Polygon.h
    )
SET ( cpps Circuit.cpp 
           Polygon.cpp 
    )
SET ( pycpps PyCif.cpp
             ${cpps}
    )

ADD_LIBRARY(cif ${cpps})
INSTALL(TARGETS cif DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/cif)

IF(Boost_FOUND)
  ADD_LIBRARY(pyCIF MODULE ${pycpps})
  SET_TARGET_PROPERTIES(pyCIF PROPERTIES
                              COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                              PREFIX        ""
                       )
  INSTALL(TARGETS pyCIF DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

