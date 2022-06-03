# -*- explicit-buffer-name: "CMakeLists.txt<Seabreeze/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${SEABREEZE_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                      )
                   set( includes     Seabreeze/SeabreezeEngine.h
                                     #Seabreeze/GraphicSeabreezeEngine.h
                      )
                   set( pyIncludes   Seabreeze/PySeabreezeEngine.h
                   #                  Seabreeze/PyGraphicSeabreezeEngine.h
                      )
                   #set( mocIncludes  Seabreeze/GraphicSeabreezeEngine.h )
                   set( cpps         SeabreezeEngine.cpp
                                     Seabreeze.cpp
                                     Node.cpp
                                     Tree.cpp
                                     #GraphicSeabreezeEngine.cpp
                      )
                   set( pyCpps       PySeabreeze.cpp
                                     PySeabreezeEngine.cpp
                   #                 PyGraphicSeabreezeEngine.cpp
                      )
          #qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${BOOKSHELF_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${UTILITIES_LIBRARY}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${Python_LIBRARIES} -lutil
                                     ${LIBEXECINFO_LIBRARIES}
                      )

           add_library( Seabreeze     ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( Seabreeze     PROPERTIES VERSION 1.0 SOVERSION 1 )
#target_link_libraries( Seabreeze     ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Seabreeze
                        "Seabreeze;${depLibs}"
                        include/coriolis2/Seabreeze
                      )

               install( TARGETS      Seabreeze       DESTINATION lib${LIB_SUFFIX} )
               #install( PROGRAMS     Seabreeze.py    DESTINATION bin RENAME Seabreeze )
               install( FILES        ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis2/Seabreeze ) 

   
