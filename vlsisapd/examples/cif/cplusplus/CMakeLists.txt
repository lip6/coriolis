INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/cif/src )
ADD_EXECUTABLE ( testCif mainCif.cpp )
TARGET_LINK_LIBRARIES ( testCif cif ) # 'testCif' is the name of the executable and 'cif' the name of the target library in cif/src/CMakeLists.txt
INSTALL ( TARGETS testCif DESTINATION bin )
