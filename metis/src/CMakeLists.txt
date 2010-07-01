
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
               install ( TARGETS       metis                      DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes} ${mocIncludes} DESTINATION include/coriolis2/metis ) 
