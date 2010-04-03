INCLUDE_DIRECTORIES(${IO_SOURCE_DIR}/dtr/src ${LIBXML2_INCLUDE_DIR})

SET ( hpps io/dtr/Techno.h
           io/dtr/Rules.h
           io/dtr/Name.h
           io/dtr/DTRException.h )

SET ( cpps Techno.cpp
           Name.cpp )

ADD_LIBRARY(dtr ${cpps})

TARGET_LINK_LIBRARIES(dtr ${LIBXML2_LIBRARIES})

INSTALL(TARGETS dtr     DESTINATION /lib)
INSTALL(FILES   ${hpps} DESTINATION /include/io/dtr)
