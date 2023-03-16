# -*- explicit-buffer-name: "CMakeLists.txt<tramontana/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${TRAMONTANA_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                      )
                   set( includes     tramontana/TramontanaEngine.h
                                     tramontana/GraphicTramontanaEngine.h
                      )
                   set( pyIncludes   tramontana/PyTramontanaEngine.h
                                     tramontana/PyGraphicTramontanaEngine.h
                      )
                   set( mocIncludes  tramontana/GraphicTramontanaEngine.h )
                   set( cpps         TramontanaEngine.cpp
                                     GraphicTramontanaEngine.cpp
                      )
                   set( pyCpps       PyTramontana.cpp
                                     PyTramontanaEngine.cpp
                                     PyGraphicTramontanaEngine.cpp
                      )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${UTILITIES_LIBRARY}
                                     ${LEFDEF_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${Python3_LIBRARIES}
                                     -lutil
                                     ${LIBEXECINFO_LIBRARIES}
                      )

           add_library( tramontana   ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( tramontana   PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( tramontana   ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Tramontana
                        "tramontana;${depLibs}"
                        include/coriolis2/tramontana
                      )

               install( TARGETS      tramontana           DESTINATION lib${LIB_SUFFIX} )
               install( FILES        ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis2/tramontana ) 

   
