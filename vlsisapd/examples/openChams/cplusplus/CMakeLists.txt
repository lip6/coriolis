INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/openChams/src ${LIBXML2_INCLUDE_DIR})
ADD_EXECUTABLE ( driveOpenChams driveOpenChams.cpp )
ADD_EXECUTABLE ( parseOpenChams parseOpenChams.cpp )
TARGET_LINK_LIBRARIES ( driveOpenChams openChams ${LIBXML_LIBRARIES}) # 'driveOpenChams' is the name of the executable and 'openChams' the name of the target library in openChams/src/CMakeLists.txt
TARGET_LINK_LIBRARIES ( parseOpenChams openChams ${LIBXML_LIBRARIES})
INSTALL ( TARGETS driveOpenChams     parseOpenChams     DESTINATION share/doc/coriolis2/examples/vlsisapd/openChams )
INSTALL ( FILES   driveOpenChams.cpp parseOpenChams.cpp DESTINATION share/doc/coriolis2/examples/vlsisapd/openChams )
INSTALL ( FILES   cmake.ex                              DESTINATION share/doc/coriolis2/examples/vlsisapd/openChams RENAME CMakeLists.txt )
