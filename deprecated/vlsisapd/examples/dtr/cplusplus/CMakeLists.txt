INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/dtr/src ${LIBXML2_INCLUDE_DIR})
ADD_EXECUTABLE ( driveDtr driveDtr.cpp )
ADD_EXECUTABLE ( parseDtr parseDtr.cpp )
TARGET_LINK_LIBRARIES ( driveDtr dtr ${LIBXML_LIBRARIES}) # 'driveDtr' is the name of the executable and 'dtr' the name of the target library in dtr/src/CMakeLists.txt
TARGET_LINK_LIBRARIES ( parseDtr dtr ${LIBXML_LIBRARIES})
INSTALL ( TARGETS driveDtr     parseDtr     DESTINATION share/doc/coriolis2/examples/vlsisapd/dtr )
INSTALL ( FILES   driveDtr.cpp parseDtr.cpp DESTINATION share/doc/coriolis2/examples/vlsisapd/dtr )
INSTALL ( FILES   cmake.ex                  DESTINATION share/doc/coriolis2/examples/vlsisapd/dtr RENAME CMakeLists.txt )
