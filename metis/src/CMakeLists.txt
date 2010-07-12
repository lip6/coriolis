
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
                   set ( cpps          MetisGraph.cpp
                                       Configuration.cpp
                                       MetisEngine.cpp
                       )

           add_library ( metis         ${cpps} ${mocCpps} )
 target_link_libraries ( metis         ${NIMBUS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${PYTHON_LIBRARIES} -lutil
                       )
               install ( TARGETS       metis                      DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes} ${mocIncludes} DESTINATION include/coriolis2/metis ) 
