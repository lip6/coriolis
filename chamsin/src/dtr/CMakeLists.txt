ADD_CUSTOM_TARGET(DTRParser echo "Creating DTRParser")

SET(source_dir ${CHAMSIN_SOURCE_DIR}/src/dtr) 
SET(binary_dir ${CHAMSIN_BINARY_DIR}/src/dtr) 

INCLUDE_DIRECTORIES(${HURRICANE_INCLUDE_DIR} ${source_dir})

ADD_CUSTOM_COMMAND(
SOURCE ${source_dir}/ParserDtrScan.ll
COMMAND ${FLEX_EXECUTABLE} 
ARGS -Pdtr -o${binary_dir}/ParserDtrScan.cpp
${source_dir}/ParserDtrScan.ll
TARGET DTRParser
OUTPUTS ${binary_dir}/ParserDtrScan.cpp)

ADD_CUSTOM_COMMAND(
SOURCE ${source_dir}/ParserDtrGram.yy
COMMAND ${BISON_EXECUTABLE} 
ARGS -d -v -p dtr -y ${source_dir}/ParserDtrGram.yy
-o ${binary_dir}/ParserDtrGram.cpp
TARGET DSTParser
DEPENDS ${binary_dir}/ParserDtrScan.cpp
OUTPUTS ${binary_dir}/ParserDtrGram.cpp)

SET(DST_SRCS ${DST_SRCS} ${binary_dir}/ParserDtrGram.cpp
${binary_dir}/ParserDtrScan.cpp)

SET_SOURCE_FILES_PROPERTIES(${binary_dir}/ParserDtrGram.cpp GENERATED)
SET_SOURCE_FILES_PROPERTIES(${binary_dir}/ParserDtrScan.cpp GENERATED)

ADD_LIBRARY(dtr SHARED
${DST_SRCS}
DtrAccess.cpp
GenericDtrAccess.cpp)

INSTALL(TARGETS dtr DESTINATION /lib)
