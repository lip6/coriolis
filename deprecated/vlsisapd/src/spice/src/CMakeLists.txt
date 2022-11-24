INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/spice/src ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/spice/Circuit.h
           vlsisapd/spice/Subckt.h
           vlsisapd/spice/Instance.h
           vlsisapd/spice/Instances.h
           vlsisapd/spice/Value.h
           vlsisapd/spice/Sources.h
           vlsisapd/spice/SpiceException.h 
    )
SET ( cpps Circuit.cpp
           Instance.cpp
           Subckt.cpp
           Value.cpp
    )
SET ( pycpps PySpice.cpp 
    )

ADD_LIBRARY(spice ${cpps})
TARGET_LINK_LIBRARIES(spice)
SET_TARGET_PROPERTIES(spice PROPERTIES VERSION 1.0 SOVERSION 1)
INSTALL(TARGETS spice DESTINATION lib${LIB_SUFFIX} )

IF(Boost_FOUND)
    ADD_LIBRARY(pySPICE MODULE ${pycpps})
    SET_TARGET_PROPERTIES(pySPICE PROPERTIES
                                  OUTPUT_NAME "SPICE"
                                  PREFIX      ""
                         )
    TARGET_LINK_LIBRARIES(pySPICE spice ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    INSTALL(TARGETS pySPICE DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/spice)
