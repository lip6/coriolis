include(${QT_USE_FILE})

qt4_wrap_cpp(MOC_SRCS AnalogicViewer.h)

include_directories(${HURRICANE_INCLUDE_DIR} ${HURRICANE_GRAPHICAL_INCLUDE_DIR}
        ${CORIOLIS_INCLUDE_DIR} ${CHAMSIN_SOURCE_DIR}/src/technology
        ${CHAMSIN_SOURCE_DIR}/src/analogic ${CHAMSIN_SOURCE_DIR}/src/device)

add_executable(atest AnalogicViewer.cpp ${MOC_SRCS} AnalogicTest.cpp)

target_link_libraries(atest atechnology analogic ${HURRICANE_LIBRARIES} ${HURRICANE_GRAPHICAL_LIBRARIES} ${QT_LIBRARIES})

install(TARGETS atest DESTINATION /bin)
