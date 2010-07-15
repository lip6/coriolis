INCLUDE_DIRECTORIES(${VLSISAPD_SOURCE_DIR}/src/agds/src ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})

SET ( hpps vlsisapd/agds/Library.h 
           vlsisapd/agds/Structure.h 
           vlsisapd/agds/Element.h 
           vlsisapd/agds/Rectangle.h 
    )
SET ( cpps Library.cpp 
           Structure.cpp 
           Rectangle.cpp 
    )
SET ( pycpps PyAgds.cpp
    )

ADD_LIBRARY(agds ${cpps})
INSTALL(TARGETS agds DESTINATION lib${LIB_SUFFIX})

INSTALL(FILES ${hpps} DESTINATION include/vlsisapd/agds)

IF(Boost_FOUND)
  ADD_LIBRARY(pyAGDS MODULE ${pycpps})
  SET_TARGET_PROPERTIES(pyAGDS PROPERTIES
                               OUTPUT_NAME "AGDS"
                               PREFIX      ""
                       )
  TARGET_LINK_LIBRARIES(pyAGDS agds ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  INSTALL(TARGETS pyAGDS DESTINATION ${PYTHON_SITE_PACKAGES})
ENDIF(Boost_FOUND)

