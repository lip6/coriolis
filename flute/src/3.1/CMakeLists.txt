# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<flute/src/3.1>" -*-

   include_directories( ${FLUTE_SOURCE_DIR}/src/3.1
                        ${CRLCORE_INCLUDE_PATH}
                        ${HURRICANE_INCLUDE_DIR}
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${PYTHON_INCLUDE_PATH}
                      )

                   set( includes      flute.h
                                      dl.h
                                      mst2.h
                                      err.h
                                      heap.h
                                      dist.h
                                      global.h
                                      neighbors.h
                      )
                   set( cpps          flute.cpp
                                      flute_mst.cpp
                                      dist.cpp
                                      dl.cpp
                                      err.cpp
                                      mst2.cpp
                                      heap.cpp
                                      neighbors.cpp
                      )
           add_library( flute         ${cpps} )
 set_target_properties( flute         PROPERTIES VERSION 3.1 SOVERSION 3 )
                   set( pyCpps        PyFlute.cpp )
                   set( depLibs       flute
                                      ${CORIOLIS_LIBRARIES}
                                      ${HURRICANE_PYTHON_LIBRARIES}
                                      ${UTILITIES_LIBRARY}
                                      ${PYTHON_LIBRARIES}
                                      -lutil
                      )

     add_python_module( "${pyCpps}"
                        None
                        "pyflute;1.0;1"
                        Flute
                        "${depLibs}"
                        include/coriolis2/flute
                      )

               install( TARGETS       flute       DESTINATION lib${LIB_SUFFIX} )
               install( FILES         ${includes} DESTINATION include/flute/3.1 ) 
               install( FILES         POST9.dat
                                      POWV9.dat   DESTINATION share/flute/3.1 ) 
