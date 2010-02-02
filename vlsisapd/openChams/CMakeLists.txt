INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/openChams ${LIBXML2_INCLUDE_DIR})

SET ( includes Circuit.h   Netlist.h   Instance.h   Net.h   Name.h   Parameters.h   Schematic.h OpenChamsException.h )
SET ( cpps     Circuit.cpp Netlist.cpp Instance.cpp Net.cpp Name.cpp Parameters.cpp Schematic.cpp )

ADD_LIBRARY(openChams ${cpps})

TARGET_LINK_LIBRARIES(openChams ${LIBXML2_LIBRARIES})

INSTALL(TARGETS openChams DESTINATION /lib)

INSTALL(FILES ${includes} DESTINATION /include/io/openChams)
