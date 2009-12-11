INCLUDE_DIRECTORIES(${CHAMS_SOURCE_DIR}/agds)

SET ( includes GdsLibrary.h GdsStructure.h GdsElement.h GdsRectangle.h )
SET ( cpps     GdsLibrary.cpp GdsStructure.cpp GdsRectangle.cpp )

ADD_LIBRARY(agds SHARED ${cpps})
INSTALL(TARGETS agds DESTINATION /lib)

INSTALL(FILES ${includes} DESTINATION /include/io/agds)
