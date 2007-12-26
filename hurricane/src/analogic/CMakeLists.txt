set(includes RdsUnit.h)  

include_directories(${HURRICANE_SOURCE_DIR}/hurricane)

add_custom_target(DTRParser echo "Creating DTRParser")

add_custom_command(
SOURCE ${HURRICANE_SOURCE_DIR}/analogic/ParserDtrScan.ll
COMMAND ${FLEX_EXECUTABLE} 
ARGS -Pdtr -o${HURRICANE_BINARY_DIR}/analogic/ParserDtrScan.cpp
${HURRICANE_SOURCE_DIR}/analogic/ParserDtrScan.ll
TARGET DTRParser
OUTPUTS ${HURRICANE_BINARY_DIR}/analogic/ParserDtrScan.cpp)

add_custom_command(
SOURCE ${HURRICANE_SOURCE_DIR}/analogic/ParserDtrGram.yy
COMMAND ${BISON_EXECUTABLE} 
ARGS -d -v -p dtr -y ${HURRICANE_SOURCE_DIR}/analogic/ParserDtrGram.yy
-o ${HURRICANE_BINARY_DIR}/analogic/ParserDtrGram.cpp
TARGET DSTParser
DEPENDS ${HURRICANE_BINARY_DIR}/analogic/ParserDtrScan.cpp
OUTPUTS ${HURRICANE_BINARY_DIR}/analogic/ParserDtrGram.cpp)

set(DST_SRCS ${DST_SRCS} ${HURRICANE_BINARY_DIR}/analogic/ParserDtrGram.cpp
${HURRICANE_BINARY_DIR}/analogic/ParserDtrScan.cpp)

SET_SOURCE_FILES_PROPERTIES(${HURRICANE_BINARY_DIR}/analogic/ParserDtrGram.cpp GENERATED)
SET_SOURCE_FILES_PROPERTIES(${HURRICANE_BINARY_DIR}/analogic/ParserDtrScan.cpp GENERATED)

INCLUDE_DIRECTORIES(${HURRICANE_BINARY_DIR}/analogic/)

add_library(analogic SHARED 
${DST_SRCS}
DtrAccess.cpp
GenericDtrAccess.cpp
GenV1Trans.cpp
MetaTransistor.cpp
RdsUnit.cpp
Transistor.cpp
TwoSpaces.cpp)

install(FILES ${includes} DESTINATION /include/hurricane) 
install(TARGETS analogic DESTINATION /lib)
