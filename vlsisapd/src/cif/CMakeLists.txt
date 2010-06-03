INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/src/cif)

SET ( includes vlsisapd/cif/CifCircuit.h   
               vlsisapd/cif/CifPolygon.h
    )
SET ( cpps CifCircuit.cpp 
           CifPolygon.cpp 
    )

ADD_LIBRARY(cif ${cpps})
INSTALL(TARGETS cif DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${includes} DESTINATION include/vlsisapd/cif)
