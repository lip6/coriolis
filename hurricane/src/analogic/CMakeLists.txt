set(includes RdsUnit.h)  

add_custom_target(DTRParser echo "Creating DTRParser")

set(source_dir ${HURRICANE_SOURCE_DIR}/src/analogic) 
set(binary_dir ${HURRICANE_BINARY_DIR}/src/analogic) 

add_custom_command(
SOURCE ${source_dir}/ParserDtrScan.ll
COMMAND ${FLEX_EXECUTABLE} 
ARGS -Pdtr -o${binary_dir}/ParserDtrScan.cpp
${source_dir}/ParserDtrScan.ll
TARGET DTRParser
OUTPUTS ${binary_dir}/ParserDtrScan.cpp)

add_custom_command(
SOURCE ${source_dir}/ParserDtrGram.yy
COMMAND ${BISON_EXECUTABLE} 
ARGS -d -v -p dtr -y ${source_dir}/ParserDtrGram.yy
-o ${binary_dir}/ParserDtrGram.cpp
TARGET DSTParser
DEPENDS ${binary_dir}/ParserDtrScan.cpp
OUTPUTS ${binary_dir}/ParserDtrGram.cpp)

set(DST_SRCS ${DST_SRCS} ${binary_dir}/ParserDtrGram.cpp
${binary_dir}/ParserDtrScan.cpp)

SET_SOURCE_FILES_PROPERTIES(${binary_dir}/ParserDtrGram.cpp GENERATED)
SET_SOURCE_FILES_PROPERTIES(${binary_dir}/ParserDtrScan.cpp GENERATED)

INCLUDE_DIRECTORIES(${binary_dir}
${HURRICANE_SOURCE_DIR}/src/hurricane
${source_dir}
)

add_library(analogic SHARED 
${DST_SRCS}
DtrAccess.cpp
GenericDtrAccess.cpp
GenV1Trans.cpp
MetaTransistor.cpp
RdsUnit.cpp
Transistor.cpp
TwoSpaces.cpp)

target_link_libraries(analogic hurricane)

install(FILES ${includes} DESTINATION /include/hurricane) 
install(TARGETS analogic DESTINATION /lib)
