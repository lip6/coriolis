# -*- explicit-buffer-name: "CMakeLists.txt<katabatic/src>" -*-

if ( CHECK_DETERMINISM )
  add_definitions ( -DCHECK_DETERMINISM ) 
endif ( CHECK_DETERMINISM )

#              include( ${QT_USE_FILE} )
   include_directories( ${KATABATIC_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${Boost_INCLUDE_DIRS}
                        ${QtX_INCLUDE_DIR}
                        ${PYTHON_INCLUDE_PATH}
                      )
                   set( includes     katabatic/Constants.h
                                     katabatic/Observer.h
                                     katabatic/Configuration.h
                                     katabatic/ChipTools.h
                                     katabatic/AutoContact.h
                                     katabatic/AutoContactTerminal.h
                                     katabatic/AutoContactTurn.h
                                     katabatic/AutoContactHTee.h
                                     katabatic/AutoContactVTee.h
                                     katabatic/AutoSegment.h          katabatic/AutoSegments.h
                                     katabatic/AutoHorizontal.h
                                     katabatic/AutoVertical.h
                                     katabatic/Grid.h                 katabatic/GridCollections.h
                                     katabatic/GridBox.h
                                     katabatic/GCell.h                katabatic/GCells.h
                                     katabatic/GCellGrid.h
                                     katabatic/Session.h
                                     katabatic/KatabaticEngine.h
                      )
                   set( mocIncludes  katabatic/GraphicKatabaticEngine.h )
                   set( cpps         Configuration.cpp
                                     Observer.cpp
                                     ChipTools.cpp
                                     AutoContact.cpp
                                     AutoContactTerminal.cpp
                                     AutoContactTurn.cpp
                                     AutoContactHTee.cpp
                                     AutoContactVTee.cpp
                                     AutoSegment.cpp
                                     AutoSegments.cpp
                                     AutoHorizontal.cpp
                                     AutoVertical.cpp
                                     Grid.cpp
                                     GCell.cpp
                                     GCellGrid.cpp
                                     PowerRails.cpp
                                     Session.cpp
                                     LayerAssign.cpp
                                     LoadGrByNet.cpp
                                     NetConstraints.cpp
                                     NetOptimals.cpp
                                     KatabaticEngine.cpp
                      )
                   set( pyCpps       PyKatabatic.cpp )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${KNIK_LIBRARIES}
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

           add_library( katabatic    ${cpps} )
 set_target_properties( katabatic    PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( katabatic    ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Katabatic
                        "katabatic;${depLibs}"
                        include/coriolis2/katabatic
                      )

               install( TARGETS katabatic     DESTINATION lib${LIB_SUFFIX} )
               install( FILES ${includes}
                              ${mocIncludes}  DESTINATION include/coriolis2/katabatic ) 
   
