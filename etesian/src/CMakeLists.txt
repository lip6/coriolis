include_directories(
  ${ETESIAN_SOURCE_DIR}/src
  ${COLOQUINTE_INCLUDE_DIR}
  ${CORIOLIS_INCLUDE_DIR}
  ${HURRICANE_INCLUDE_DIR}
  ${CONFIGURATION_INCLUDE_DIR}
  ${QtX_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${Python_INCLUDE_DIRS})
set(includes
    etesian/Configuration.h
    etesian/Placement.h
    etesian/FeedCells.h
    etesian/BufferCells.h
    etesian/BloatCells.h
    etesian/BloatProperty.h
    etesian/EtesianEngine.h
    etesian/GraphicEtesianEngine.h)
set(pyIncludes etesian/PyEtesianEngine.h etesian/PyGraphicEtesianEngine.h)
set(mocIncludes etesian/GraphicEtesianEngine.h)
set(cpps
    Configuration.cpp
    HFNS.cpp
    AddFeeds.cpp
    Placement.cpp
    FlattenPower.cpp
    FeedCells.cpp
    BufferCells.cpp
    BloatCells.cpp
    BloatProperty.cpp
    EtesianEngine.cpp
    GraphicEtesianEngine.cpp)
set(pyCpps PyEtesian.cpp PyEtesianEngine.cpp PyGraphicEtesianEngine.cpp)
qtx_wrap_cpp(mocCpps ${mocIncludes})
set(depLibs
    ${KATABATIC_LIBRARIES}
    ${CORIOLIS_PYTHON_LIBRARIES}
    ${CORIOLIS_LIBRARIES}
    ${HURRICANE_PYTHON_LIBRARIES}
    ${HURRICANE_GRAPHICAL_LIBRARIES}
    ${HURRICANE_LIBRARIES}
    ${KITE_LIBRARIES}
    ${CONFIGURATION_LIBRARY}
    ${BOOKSHELF_LIBRARY}
    ${CIF_LIBRARY}
    ${AGDS_LIBRARY}
    ${UTILITIES_LIBRARY}
    ${LEFDEF_LIBRARIES}
    ${COLOQUINTE_LIBRARIES}
    ${OA_LIBRARIES}
    ${QtX_LIBRARIES}
    ${Boost_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${Python3_LIBRARIES}
    -lutil
    ${LIBEXECINFO_LIBRARIES})

add_library(etesian ${cpps} ${mocCpps} ${pyCpps})
set_target_properties(etesian PROPERTIES VERSION 1.0 SOVERSION 1)
target_link_libraries(etesian ${depLibs})

add_python_module("${pyCpps}" "${pyIncludes}" "Do_not_generate_C_library"
                  Etesian "etesian;${depLibs}" include/coriolis2/etesian)

install(TARGETS etesian DESTINATION lib${LIB_SUFFIX})
install(FILES ${includes} ${mocIncludes} DESTINATION include/coriolis2/etesian)
