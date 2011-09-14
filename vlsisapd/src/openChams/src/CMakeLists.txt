INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/openChams/src ${LIBXML2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/openChams/Circuit.h
           vlsisapd/openChams/Netlist.h
           vlsisapd/openChams/Instance.h
           vlsisapd/openChams/Device.h
           vlsisapd/openChams/Net.h
           vlsisapd/openChams/Name.h
           vlsisapd/openChams/Operator.h
           vlsisapd/openChams/Parameters.h
           vlsisapd/openChams/Schematic.h
           vlsisapd/openChams/SimulModel.h
           vlsisapd/openChams/Sizing.h
           vlsisapd/openChams/Layout.h
           vlsisapd/openChams/Node.h
           vlsisapd/openChams/Transistor.h
           vlsisapd/openChams/Port.h
           vlsisapd/openChams/Wire.h
           vlsisapd/openChams/OpenChamsException.h 
    )
SET ( cpps Circuit.cpp
           Netlist.cpp
           Instance.cpp
           Device.cpp
           Net.cpp
           Name.cpp
           Operator.cpp
           Parameters.cpp
           Schematic.cpp
           SimulModel.cpp
           Sizing.cpp
           Layout.cpp
           Node.cpp
           Transistor.cpp 
           Wire.cpp
    )
SET ( pycpps PyOpenChams.cpp 
    )

ADD_LIBRARY(openChams ${cpps})
TARGET_LINK_LIBRARIES(openChams ${LIBXML2_LIBRARIES})
SET_TARGET_PROPERTIES(openChams PROPERTIES VERSION 1.0 SOVERSION 1)
INSTALL(TARGETS openChams DESTINATION lib${LIB_SUFFIX} )

IF(Boost_FOUND)
    ADD_LIBRARY(pyOPENCHAMS MODULE ${pycpps})
    SET_TARGET_PROPERTIES(pyOPENCHAMS PROPERTIES
                                      OUTPUT_NAME "OPENCHAMS"
                                      PREFIX      ""
                         )
    TARGET_LINK_LIBRARIES(pyOPENCHAMS openChams ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    INSTALL(TARGETS pyOPENCHAMS DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/openChams)
