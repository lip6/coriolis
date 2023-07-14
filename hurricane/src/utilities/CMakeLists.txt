

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/utilities
                       )

                   set ( includes  hurricane/utilities/Path.h
                                   hurricane/utilities/Dots.h
                       )             
                   set ( cpps      Path.cpp
                                   Dots.cpp
                       )             
                         
           add_library ( utils     ${cpps} )
 set_target_properties ( utils     PROPERTIES VERSION 1.0 SOVERSION 1 )

               install ( TARGETS   utils DESTINATION lib${LIB_SUFFIX} )
               install ( FILES     ${includes}
                                   DESTINATION include/coriolis2/hurricane/utilities ) 

