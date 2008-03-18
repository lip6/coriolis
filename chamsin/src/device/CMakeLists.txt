INCLUDE_DIRECTORIES(${HURRICANE_INCLUDE_DIR}
${CHAMSIN_SOURCE_DIR}/src/dtr ${CHAMSIN_SOURCE_DIR}/src/analogic)

ADD_LIBRARY(device SHARED Device.cpp TrMos.cpp TrMos_PlaceRoute.cpp)

TARGET_LINK_LIBRARIES(device analogic dtr hurricane)

INSTALL(TARGETS device DESTINATION /lib)
