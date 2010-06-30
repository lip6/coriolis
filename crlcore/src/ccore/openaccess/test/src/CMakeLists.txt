
INCLUDE_DIRECTORIES ( ${HURRICANE_INCLUDE_DIR}
                      ${CORIOLIS_INCLUDE_DIR} 
                      ${HURRICANEAMS_INCLUDE_DIR}
                      ${HURRICANEAMS_GRAPHICAL_INCLUDE_DIR}
                      ${AMSCORE_INCLUDE_DIR}
                      ${SCHEMATIC_SOURCE_DIR}/src
                    )

SET ( CPPS main.cpp )

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
ADD_EXECUTABLE(testOAWrapper ${QRCS_SRCS} ${MOCS} ${CPPS} ${QRCS})
TARGET_LINK_LIBRARIES(testOAWrapper ${HURRICANE_LIBRARIES} 
                                ${HURRICANE_PYTHON_LIBRARIES}
                                ${OPENCHAMS_LIBRARY}
                                ${CIF_LIBRARY}
                                ${AGDS_LIBRARY}
                                ${CORIOLIS_LIBRARIES}
                                ${HURRICANEAMS_LIBRARIES}
                                ${AMSCORE_LIBRARIES}
                                ${OA_LIBRARIES} )
INSTALL(TARGETS testOAWrapper DESTINATION bin)

