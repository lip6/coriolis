INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/cif/src ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/cif/Circuit.h   
           vlsisapd/cif/Polygon.h
    )
SET ( cpps Circuit.cpp 
           Polygon.cpp 
    )
SET ( pycpps PyCif.cpp
    )

ADD_LIBRARY(cif ${cpps})
SET_TARGET_PROPERTIES(cif PROPERTIES VERSION 1.0 SOVERSION 1)
INSTALL(TARGETS cif DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/cif)

IF(Boost_FOUND)
  ADD_LIBRARY(pyCIF MODULE ${pycpps})
  SET_TARGET_PROPERTIES(pyCIF PROPERTIES
                              OUTPUT_NAME "CIF"
                              PREFIX      ""
                       )
  TARGET_LINK_LIBRARIES(pyCIF cif ${Boost_LIBRARIES} )
  INSTALL(TARGETS pyCIF DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

