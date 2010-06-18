
if ( HMETIS_FOUND )
  add_definitions ( -DHAVE_HMETIS_LIB )
endif ( HMETIS_FOUND )

               include ( ${QT_USE_FILE} )

   include_directories ( ${METIS_SOURCE_DIR}/src
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                       )
                   set ( includes      metis/hmetis.h
                                       metis/MetisGraph.h
                                       metis/Configuration.h
                                       metis/MetisEngine.h
                       )
#                  set ( mocIncludes   metis/ConfigurationWidget.h
#                                      metis/GraphicMaukaEngine.h )
                   set ( cpps          MetisGraph.cpp
                                       Configuration.cpp
                                       MetisEngine.cpp
                       )
#         qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( metis         ${cpps} ${mocCpps} )
 target_link_libraries ( metis         ${HMETIS_LIBRARIES}
                                       ${NIMBUS_LIBRARIES}
                                       ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                       )
               install ( TARGETS       metis                      DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes} ${mocIncludes} DESTINATION include/coriolis2/metis ) 
