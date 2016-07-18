# -*- explicit-buffer-name: "CMakeLists.txt<anabatic/src>" -*-

if ( CHECK_DETERMINISM )
  add_definitions ( -DCHECK_DETERMINISM ) 
endif ( CHECK_DETERMINISM )

   include_directories( ${ANABATIC_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${Boost_INCLUDE_DIRS}
                        ${QtX_INCLUDE_DIR}
                        ${PYTHON_INCLUDE_PATH}
                      )
                   set( includes     anabatic/Constants.h
                                     anabatic/Configuration.h
                                     anabatic/Matrix.h
                                     anabatic/Edge.h                 anabatic/Edges.h
                                     anabatic/GCell.h               #anabatic/GCells.h
                                     anabatic/AnabaticEngine.h
                                     anabatic/GraphicAnabaticEngine.h
                                     anabatic/Dijkstra.h

                                     anabatic/AutoContact.h
                                     anabatic/AutoContactTerminal.h
                                     anabatic/AutoContactTurn.h
                                     anabatic/AutoContactHTee.h
                                     anabatic/AutoContactVTee.h
                                     anabatic/AutoSegment.h          anabatic/AutoSegments.h
                                     anabatic/AutoHorizontal.h
                                     anabatic/AutoVertical.h
                                     anabatic/Session.h
                      )
                   set( mocIncludes  anabatic/GraphicAnabaticEngine.h )
                   set( pyIncludes   anabatic/PyAnabaticEngine.h
                                     anabatic/PyGraphicAnabaticEngine.h
                      )
                   set( cpps         Constants.cpp
                                     Configuration.cpp
                                     Matrix.cpp
                                     Edge.cpp
                                     Edges.cpp
                                     GCell.cpp
                                     AnabaticEngine.cpp
                                     GlobalRoute.cpp
                                     GraphicAnabaticEngine.cpp
                                     Dijkstra.cpp

                                     AutoContact.cpp
                                     AutoContactTerminal.cpp
                                     AutoContactTurn.cpp
                                     AutoContactHTee.cpp
                                     AutoContactVTee.cpp
                                     AutoSegment.cpp          AutoSegments.cpp
                                     AutoHorizontal.cpp
                                     AutoVertical.cpp
                                     Session.cpp
                                     NetConstraints.cpp
                                     NetOptimals.cpp
                                     LoadGlobalRouting.cpp
                      )
                   set( pyCpps       PyAnabaticEngine.cpp
                                     PyGraphicAnabaticEngine.cpp
                                     PyAnabatic.cpp
                      )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${PYTHON_LIBRARIES} -lutil
                      )

           add_library( anabatic    ${cpps} ${mocCpps} )
 set_target_properties( anabatic    PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( anabatic    ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "pyanabatic;1.0;1"
                        Anabatic
                        "anabatic;${depLibs}"
                        include/coriolis2/anabatic
                      )

               install( TARGETS anabatic     DESTINATION lib${LIB_SUFFIX} )
               install( FILES ${includes}
                              ${mocIncludes}  DESTINATION include/coriolis2/anabatic ) 
   
