INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/agds/src )
ADD_EXECUTABLE ( driveAgds driveAgds.cpp )
TARGET_LINK_LIBRARIES ( driveAgds agds ) # 'driveAgds' is the name of the executable and 'agds' the name of the target library in agds/src/CMakeLists.txt
INSTALL ( TARGETS driveAgds     DESTINATION share/doc/coriolis2/examples/vlsisapd/agds )
INSTALL ( FILES   driveAgds.cpp DESTINATION share/doc/coriolis2/examples/vlsisapd/agds )
INSTALL ( FILES   cmake.ex      DESTINATION share/doc/coriolis2/examples/vlsisapd/agds RENAME CMakeLists.txt )
