INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/cif/src )
ADD_EXECUTABLE ( driveCif driveCif.cpp )
TARGET_LINK_LIBRARIES ( driveCif cif ) # 'driveCif' is the name of the executable and 'cif' the name of the target library in cif/src/CMakeLists.txt
INSTALL ( TARGETS driveCif     DESTINATION share/doc/coriolis2/examples/vlsisapd/cif )
INSTALL ( FILES   driveCif.cpp DESTINATION share/doc/coriolis2/examples/vlsisapd/cif )
INSTALL ( FILES   cmake.ex     DESTINATION share/doc/coriolis2/examples/vlsisapd/cif RENAME CMakeLists.txt )
