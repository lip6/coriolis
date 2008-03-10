INCLUDE_DIRECTORIES(${CHAMSIN_SOURCE_DIR}/src/dtr ${HURRICANE_INCLUDE_DIR} ${source_dir})

ADD_LIBRARY(analogic SHARED 
GenV1Trans.cpp
MetaTransistor.cpp
RdsUnit.cpp
Transistor.cpp
TwoSpaces.cpp)

TARGET_LINK_LIBRARIES(analogic ${HURRICANE_LIBRARIES})

INSTALL(TARGETS analogic DESTINATION /lib)
