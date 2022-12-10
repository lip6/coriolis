# -*- explicit-buffer-name: "CMakeLists.txt<tutorial/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${TUTORIAL_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                      )
                   set( includes     tutorial/TutorialEngine.h
                                     tutorial/GraphicTutorialEngine.h
                      )
                   set( pyIncludes   tutorial/PyTutorialEngine.h
                                     tutorial/PyGraphicTutorialEngine.h
                      )
                   set( mocIncludes  tutorial/GraphicTutorialEngine.h )
                   set( cpps         TutorialEngine.cpp
                                     GraphicTutorialEngine.cpp
                      )
                   set( pyCpps       PyTutorial.cpp
                                     PyTutorialEngine.cpp
                                     PyGraphicTutorialEngine.cpp
                      )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

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

           add_library( tutorial     ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( tutorial     PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( tutorial     ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Tutorial
                        "tutorial;${depLibs}"
                        include/coriolis2/tutorial
                      )

               install( TARGETS      tutorial       DESTINATION lib${LIB_SUFFIX} )
               install( PROGRAMS     tutorial.py    DESTINATION bin RENAME tutorial )
               install( FILES        ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis2/tutorial ) 

   
