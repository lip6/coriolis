INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/openChams/src ${LIBXML2_INCLUDE_DIR})

SET ( includes vlsisapd/openChams/Circuit.h
               vlsisapd/openChams/Netlist.h
               vlsisapd/openChams/Instance.h
               vlsisapd/openChams/Net.h
               vlsisapd/openChams/Name.h
               vlsisapd/openChams/Operator.h
               vlsisapd/openChams/Parameters.h
               vlsisapd/openChams/Schematic.h
               vlsisapd/openChams/Sizing.h
               vlsisapd/openChams/Transistor.h
               vlsisapd/openChams/OpenChamsException.h 
    )
SET ( cpps Circuit.cpp
           Netlist.cpp
           Instance.cpp
           Net.cpp
           Name.cpp
           Operator.cpp
           Parameters.cpp
           Schematic.cpp
           Sizing.cpp
           Transistor.cpp 
    )

ADD_LIBRARY(openChams ${cpps})

TARGET_LINK_LIBRARIES(openChams ${LIBXML2_LIBRARIES})

INSTALL(TARGETS openChams DESTINATION lib${LIB_SUFFIX} )

INSTALL(FILES ${includes} DESTINATION include/vlsisapd/openChams)
