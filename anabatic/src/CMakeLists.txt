# -*- explicit-buffer-name: "CMakeLists.txt<anabatic/src>" -*-

if ( CHECK_DETERMINISM )
  add_definitions ( -DCHECK_DETERMINISM ) 
endif ( CHECK_DETERMINISM )

   include_directories( ${ANABATIC_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${FLUTE_INCLUDE_DIR} 
                        ${Boost_INCLUDE_DIRS}
                        ${QtX_INCLUDE_DIR}
                        ${Python_INCLUDE_DIRS}
                      )
                   set( includes     anabatic/Constants.h
                                     anabatic/Configuration.h
                                     anabatic/Matrix.h
                                     anabatic/EdgeCapacity.h
                                     anabatic/Edge.h                 anabatic/Edges.h
                                     anabatic/GCell.h               #anabatic/GCells.h
                                     anabatic/AnabaticEngine.h
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
                                     anabatic/NetBuilder.h
                                     anabatic/NetBuilderM2.h
                                     anabatic/NetBuilderHV.h
                                     anabatic/NetBuilderVH.h
                                     anabatic/ChipTools.h
                      )
                   set( pyIncludes   )
                   set( cpps         Constants.cpp
                                     Configuration.cpp
                                     Matrix.cpp
                                     EdgeCapacity.cpp
                                     Edge.cpp
                                     Edges.cpp
                                     GCell.cpp
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
                                     NetBuilder.cpp
                                     NetBuilderM2.cpp
                                     NetBuilderHV.cpp
                                     NetBuilderVH.cpp
                                     ChipTools.cpp
                                     LayerAssign.cpp
                                     AntennaProtect.cpp
                                     PreRouteds.cpp
                                     AnabaticEngine.cpp
                      )
                   set( pyCpps       PyAnabatic.cpp
                      )

                   set( depLibs      ${ETESIAN_LIBRARIES}
                                     ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${FLUTE_LIBRARIES}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${Python_LIBRARIES} -lutil
                      )

           add_library( anabatic    ${cpps} )
 set_target_properties( anabatic    PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( anabatic    ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "pyanabatic;1.0;1"
                        Anabatic
                        "anabatic;${depLibs}"
                        include/coriolis2/anabatic
                      )

               install( TARGETS anabatic      DESTINATION lib${LIB_SUFFIX} )
               install( FILES ${includes}     DESTINATION include/coriolis2/anabatic ) 
   
