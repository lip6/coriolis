INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/dtr/src ${LIBXML2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/dtr/Techno.h
           vlsisapd/dtr/Rules.h
           vlsisapd/dtr/DTRException.h 
    )

SET ( cpps Techno.cpp
    )

SET ( pycpps PyDtr.cpp
    )

ADD_LIBRARY(dtr ${cpps})
TARGET_LINK_LIBRARIES(dtr ${LIBXML2_LIBRARIES})
SET_TARGET_PROPERTIES(dtr PROPERTIES VERSION 1.0 SOVERSION 1)
INSTALL(TARGETS dtr DESTINATION lib${LIB_SUFFIX})

IF (Boost_FOUND)
    ADD_LIBRARY(pyDTR MODULE ${pycpps})
    SET_TARGET_PROPERTIES(pyDTR PROPERTIES
                                OUTPUT_NAME "DTR"
                                PREFIX      ""
                          )
    TARGET_LINK_LIBRARIES(pyDTR dtr ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} )
    INSTALL(TARGETS pyDTR   DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/dtr)
