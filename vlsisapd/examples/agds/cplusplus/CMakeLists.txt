INCLUDE_DIRECTORIES ( ${VLSISAPD_SOURCE_DIR}/src/agds/src )
ADD_EXECUTABLE ( testAgds mainAgds.cpp )
TARGET_LINK_LIBRARIES ( testAgds agds ) # 'testAgds' is the name of the executable and 'agds' the name of the target library in agds/src/CMakeLists.txt
INSTALL ( TARGETS testAgds DESTINATION bin )
