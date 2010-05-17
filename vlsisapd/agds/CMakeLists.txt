INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/agds)

SET ( includes GdsLibrary.h GdsStructure.h GdsElement.h GdsRectangle.h )
SET ( cpps     GdsLibrary.cpp GdsStructure.cpp GdsRectangle.cpp )

ADD_LIBRARY(agds ${cpps})
INSTALL(TARGETS agds DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${includes} DESTINATION include/io/agds)
