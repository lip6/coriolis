include(${QT_USE_FILE})

qt4_wrap_cpp(MOC_TRANSISTOR TransistorViewer.h)
qt4_wrap_cpp(MOC_CAPACITOR CapacitorViewer.h)

include_directories(${HURRICANE_INCLUDE_DIR} ${HURRICANE_GRAPHICAL_INCLUDE_DIR}
        ${CORIOLIS_INCLUDE_DIR} ${CHAMSIN_SOURCE_DIR}/src/technology
        ${CHAMSIN_SOURCE_DIR}/src/analogic ${CHAMSIN_SOURCE_DIR}/src/device)

add_executable(transview TransistorViewer.cpp ${MOC_TRANSISTOR} TransistorTest.cpp)
add_executable(capaview CapacitorViewer.cpp  ${MOC_CAPACITOR}  CapacitorTest.cpp)

target_link_libraries(transview atechnology analogic ${HURRICANE_LIBRARIES} ${HURRICANE_GRAPHICAL_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(capaview   atechnology analogic ${HURRICANE_LIBRARIES} ${HURRICANE_GRAPHICAL_LIBRARIES} ${QT_LIBRARIES})

install(TARGETS transview capaview DESTINATION /bin)
