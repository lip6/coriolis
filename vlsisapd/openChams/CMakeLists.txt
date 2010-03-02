INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/openChams ${LIBXML2_INCLUDE_DIR})

SET ( includes
        Circuit.h
        Netlist.h
        Instance.h
        Net.h
        Name.h
        Operator.h
        Parameters.h
        Schematic.h
        Sizing.h
        Transistor.h
        OpenChamsException.h )
SET ( cpps
        Circuit.cpp
        Netlist.cpp
        Instance.cpp
        Net.cpp
        Name.cpp
        Operator.cpp
        Parameters.cpp
        Schematic.cpp
        Sizing.cpp
        Transistor.cpp )

ADD_LIBRARY(openChams ${cpps})

TARGET_LINK_LIBRARIES(openChams ${LIBXML2_LIBRARIES})

INSTALL(TARGETS openChams DESTINATION /lib)

INSTALL(FILES ${includes} DESTINATION /include/io/openChams)
