INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/cif)

SET ( includes CifCircuit.h   CifPolygon.h )
SET ( cpps     CifCircuit.cpp CifPolygon.cpp )

ADD_LIBRARY(cif ${cpps})
INSTALL(TARGETS cif DESTINATION /lib)

INSTALL(FILES ${includes} DESTINATION /include/io/cif)
