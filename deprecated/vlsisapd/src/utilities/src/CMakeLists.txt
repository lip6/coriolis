

   include_directories ( ${VLSISAPD_SOURCE_DIR}/src/utilities/src
                       )

                   set ( includes        vlsisapd/utilities/Path.h
                                         vlsisapd/utilities/Dots.h
                       )             
                   set ( cpps            Path.cpp
                                         Dots.cpp
                       )             
                         
           add_library ( vlsisapdutils   ${cpps} )
 set_target_properties ( vlsisapdutils   PROPERTIES VERSION 1.0 SOVERSION 1 )

               install ( TARGETS         vlsisapdutils DESTINATION lib${LIB_SUFFIX} )
               install ( FILES           ${includes}
                                         DESTINATION include/vlsisapd/utilities ) 

