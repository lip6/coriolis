# -*- explicit-buffer-name: "CMakeLists.txt<foehn/src>" -*-

   include_directories( ${FOEHN_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${ETESIAN_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${FLUTE_INCLUDE_DIR} 
                        ${Boost_INCLUDE_DIRS}
                        ${QtX_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                      )
                   set( includes     foehn/DagProperty.h
                                     foehn/Dag.h
                                     foehn/Configuration.h
                                     foehn/FoehnEngine.h
                      )
                   set( pyIncludes   foehn/PyDagExtension.h
		                     foehn/PyDag.h
		                     foehn/PyFoehnEngine.h )
                   set( cpps         DagProperty.cpp
                                     Dag.cpp
                                     Configuration.cpp
                                     FoehnEngine.cpp
                      )
                   set( pyCpps       PyDagExtension.cpp
		                     PyDag.cpp
		                     PyFoehnEngine.cpp
		                     PyFoehn.cpp
                      )

                   set( depLibs      ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${COLOQUINTE_LIBRARIES}
                                     ${FLUTE_LIBRARIES}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${Python_LIBRARIES} -lutil
                      )

           add_library( foehn    ${cpps} )
 set_target_properties( foehn    PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( foehn    ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "pyfoehn;1.0;1"
                        Foehn
                        "foehn;${depLibs}"
                        include/coriolis2/foehn
                      )

               install( TARGETS foehn      DESTINATION lib${LIB_SUFFIX} )
               install( FILES ${includes}  DESTINATION include/coriolis2/foehn ) 
   
