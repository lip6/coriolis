include(${QT_USE_FILE})

include_directories(${HURRICANE_INCLUDE_DIR} ${CORIOLIS_INCLUDE_DIR}
${CHAMSIN_SOURCE_DIR}/src/technology
${CHAMSIN_SOURCE_DIR}/src/analogic ${CHAMSIN_SOURCE_DIR}/src/device)

add_executable(atest AnalogicTest.cpp)

target_link_libraries(atest atechnology analogic ${HURRICANE_LIBRARIES} ${QT_LIBRARIES})

install(TARGETS atest DESTINATION /bin)
