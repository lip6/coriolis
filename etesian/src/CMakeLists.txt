# -*- explicit-buffer-name: "CMakeLists.txt<etesian/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${ETESIAN_SOURCE_DIR}/src
                        ${COLOQUINTE_INCLUDE_DIR} 
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${PYTHON_INCLUDE_PATH}
                      )
                   set( includes      etesian/Configuration.h
                                      etesian/FeedCells.h
                                      etesian/EtesianEngine.h
                                      etesian/GraphicEtesianEngine.h
                      )               
                   set( pyIncludes    etesian/PyEtesianEngine.h
                                      etesian/PyGraphicEtesianEngine.h
                      )
                   set( mocIncludes   etesian/GraphicEtesianEngine.h )
                   set( cpps          Configuration.cpp
                                      AddFeeds.cpp
                                      FeedCells.cpp
                                      EtesianEngine.cpp
                                      GraphicEtesianEngine.cpp
                      )
                   set( pyCpps        PyEtesian.cpp
                                      PyEtesianEngine.cpp
                                      PyGraphicEtesianEngine.cpp
                      )
          qtX_wrap_cpp( mocCpps       ${mocIncludes} )
                   set( depLibs       ${KATABATIC_LIBRARIES}
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
                                      ${PYTHON_LIBRARIES} -lutil
                                      ${LIBEXECINFO_LIBRARIES}
                      )

           add_library( etesian       ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( etesian       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( etesian       ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Etesian
                        "etesian;${depLibs}"
                        include/coriolis2/etesian
                      )

               install( TARGETS       etesian           DESTINATION lib${LIB_SUFFIX} )
               install( FILES         ${includes}
                                      ${mocIncludes}    DESTINATION include/coriolis2/etesian ) 
   
