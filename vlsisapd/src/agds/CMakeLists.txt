INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/agds)

SET ( includes vlsisapd/agds/GdsLibrary.h 
               vlsisapd/agds/GdsStructure.h 
               vlsisapd/agds/GdsElement.h 
               vlsisapd/agds/GdsRectangle.h 
    )
SET ( cpps GdsLibrary.cpp 
           GdsStructure.cpp 
           GdsRectangle.cpp 
    )

ADD_LIBRARY(agds ${cpps})
INSTALL(TARGETS agds DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${includes} DESTINATION include/vlsisapd/agds)
