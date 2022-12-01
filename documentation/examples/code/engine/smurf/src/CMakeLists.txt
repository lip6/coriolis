# -*- explicit-buffer-name: "CMakeLists.txt<smurf/src>" -*-

#              include ( ${QT_USE_FILE} )

   include_directories ( ${SMURF_SOURCE_DIR}/src
                         ${AXEP_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                         ${QtX_INCLUDE_DIRS}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )
                   set ( includes      smurf/Configuration.h
                                       smurf/SmurfEngine.h
                                       smurf/GraphicSmurfEngine.h
                       )	       
                   set ( pyIncludes    smurf/PySmurfEngine.h
                                       smurf/PyGraphicSmurfEngine.h
                       )
		   set ( mocIncludes   smurf/GraphicSmurfEngine.h )
                   set ( cpps          Configuration.cpp
                                       SmurfEngine.cpp
                                       GraphicSmurfEngine.cpp
                       )
                   set ( pyCpps        PySmurf.cpp
                                       PySmurfEngine.cpp
                                       PyGraphicSmurfEngine.cpp
                       )
          qt5_wrap_cpp ( mocCpps       ${mocIncludes} )

           add_library ( smurf         ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties ( smurf         PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( smurf         ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${BOOKSHELF_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QtX_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${Python_LIBRARIES} -lutil
                                       ${LIBEXECINFO_LIBRARIES}
                       )

           add_library ( pySmurf       MODULE ${pyCpps} )
 set_target_properties ( pySmurf       PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                                  PREFIX        ""
                                                  OUTPUT_NAME   "Smurf"
                       )

 target_link_libraries ( pySmurf       smurf
                                       ${CORIOLIS_PYTHON_LIBRARIES}
                       )
               install ( TARGETS       smurf           DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS       pySmurf         DESTINATION ${PYTHON_SITE_PACKAGES} )

               install ( FILES         ${includes}
                                       ${mocIncludes}
                                       ${pyIncludes}   DESTINATION include/coriolis2/smurf ) 
   
