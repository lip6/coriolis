# -*- explicit-buffer-name: "CMakeLists.txt<katana/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${KATANA_SOURCE_DIR}/src
                        ${ANABATIC_INCLUDE_DIR} 
                        ${ETESIAN_INCLUDE_DIR} 
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${FLUTE_INCLUDE_DIR}
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                      )
      find_package (Python 3 COMPONENTS Interpreter Development.Module )
                   set( includes     katana/Constants.h
                                     katana/Block.h
                                     katana/TrackCost.h
                                     katana/DataNegociate.h
                                     katana/DataSymmetric.h
                                     katana/TrackElement.h       katana/TrackElements.h
                                     katana/TrackSegment.h
                                     katana/TrackSegmentRegular.h
                                     katana/TrackSegmentNonPref.h
                                     katana/TrackSegmentWide.h
                                     katana/TrackFixedSegment.h
                                     katana/TrackMarker.h
                                     katana/Track.h
                                     katana/Tracks.h
                                     katana/HorizontalTrack.h
                                     katana/VerticalTrack.h
                                     katana/RoutingPlane.h
                                     katana/Session.h
                                     katana/Manipulator.h
                                     katana/SegmentFsm.h
                                     katana/RoutingEvent.h
                                     katana/RoutingEventQueue.h
                                     katana/RoutingEventHistory.h
                                     katana/RoutingEventLoop.h
                                     katana/NegociateWindow.h
                                     katana/Configuration.h
                                     katana/KatanaEngine.h
                                     katana/GraphicKatanaEngine.h
                      )
                   set( pyIncludes   katana/PyKatanaEngine.h
                                     katana/PyGraphicKatanaEngine.h
                                     katana/PyKatanaFlags.h
                      )
                   set( mocIncludes  katana/GraphicKatanaEngine.h )
                   set( cpps         Constants.cpp
                                     Configuration.cpp
                                     Block.cpp
                                     DataNegociate.cpp
                                     DataSymmetric.cpp
                                     TrackCost.cpp
                                     TrackElement.cpp
                                     TrackElements.cpp
                                     TrackSegment.cpp
                                     TrackSegmentRegular.cpp
                                     TrackSegmentNonPref.cpp
                                     TrackSegmentWide.cpp
                                     TrackFixedSegment.cpp
                                     TrackMarker.cpp
                                     Track.cpp
                                     Tracks.cpp
                                     HorizontalTrack.cpp
                                     VerticalTrack.cpp
                                     RoutingPlane.cpp
                                     Session.cpp
                                     Manipulator.cpp
                                     SegmentFsm.cpp
                                     RoutingEvent.cpp
                                     RoutingEventQueue.cpp
                                     RoutingEventHistory.cpp
                                     RoutingEventLoop.cpp
                                     NegociateWindow.cpp
                                     PowerRails.cpp
                                     PreRouteds.cpp
                                     ProtectRoutingPads.cpp
                                     PreProcess.cpp
                                     BloatProfile.cpp
                                     GlobalRoute.cpp
                                     SymmetricRoute.cpp
                                     KatanaEngine.cpp
                                     GraphicKatanaEngine.cpp
                      )
                   set( pyCpps       PyKatana.cpp
                                     PyKatanaEngine.cpp
                                     PyGraphicKatanaEngine.cpp
                                     PyKatanaFlags.cpp
                      )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${ETESIAN_LIBRARIES}
                                     ${ANABATIC_LIBRARIES}
                                     ${CORIOLIS_PYTHON_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${BOOKSHELF_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${COLOQUINTE_LIBRARIES}
                                     ${UTILITIES_LIBRARY}
                                     ${FLUTE_LIBRARIES}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${Python_LIBRARIES}
                                      -lutil
                                     ${LIBEXECINFO_LIBRARIES}
                      )

           add_library( katana       ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( katana       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( katana       ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Katana
                        "katana;${depLibs}"
                        include/coriolis2/katana
                      )

               install( TARGETS      katana           DESTINATION lib${LIB_SUFFIX} )
               install( FILES        ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis2/katana ) 

   
