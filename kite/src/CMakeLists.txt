# -*- explicit-buffer-name: "CMakeLists.txt<kite/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${KITE_SOURCE_DIR}/src
                        ${CORIOLIS_INCLUDE_DIR} 
                        ${HURRICANE_INCLUDE_DIR} 
                        ${CONFIGURATION_INCLUDE_DIR} 
                        ${WtX_INCLUDE_DIR}
                        ${Boost_INCLUDE_DIRS}
                        ${PYTHON_INCLUDE_PATH}
                      )
                   set( includes     kite/Constants.h
                                     kite/TrackCost.h
                                     kite/DataNegociate.h
                                     kite/TrackElement.h       kite/TrackElements.h
                                     kite/TrackSegment.h
                                     kite/TrackFixedSegment.h
                                     kite/TrackMarker.h
                                     kite/Track.h
                                     kite/Tracks.h
                                     kite/HorizontalTrack.h
                                     kite/VerticalTrack.h
                                     kite/RoutingPlane.h
                                     kite/Session.h
                                     kite/Manipulator.h
                                     kite/SegmentFsm.h
                                     kite/RoutingEvent.h
                                     kite/RoutingEventQueue.h
                                     kite/RoutingEventHistory.h
                                     kite/RoutingEventLoop.h
                                     kite/NegociateWindow.h
                                     kite/Configuration.h
                                     kite/KiteEngine.h
                                     kite/GraphicKiteEngine.h
                      )
                   set( pyIncludes   kite/PyKiteEngine.h
                                     kite/PyGraphicKiteEngine.h
                      )
                   set( mocIncludes  kite/GraphicKiteEngine.h )
                   set( cpps         TrackCost.cpp
                                     DataNegociate.cpp
                                     TrackElement.cpp
                                     TrackElements.cpp
                                     TrackSegment.cpp
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
                                     BuildPowerRails.cpp
                                     BuildPreRouteds.cpp
                                     ProtectRoutingPads.cpp
                                     PreProcess.cpp
                                     Configuration.cpp
                                     KiteEngine.cpp
                                     GraphicKiteEngine.cpp
                      )
                   set( pyCpps       PyKite.cpp
                                     PyKiteEngine.cpp
                                     PyGraphicKiteEngine.cpp
                      )
                   set( kiteCpps     KiteMain.cpp )
          qtX_wrap_cpp( mocCpps      ${mocIncludes} )

                   set( depLibs      ${KATABATIC_LIBRARIES}
                                     ${KNIK_LIBRARIES}
                                     ${NIMBUS_LIBRARIES}
                                     ${CORIOLIS_PYTHON_LIBRARIES}
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
                                     ${PYTHON_LIBRARIES} -lutil
                                     ${LIBEXECINFO_LIBRARIES}
                      )

           add_library( kite         ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( kite         PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( kite         ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Kite
                        "kite;${depLibs}"
                        include/coriolis2/kite
                      )

#       add_executable( kite.bin     ${kiteCpps} )
#target_link_libraries( kite.bin     kite ${depLibs} )

               install( TARGETS      kite           DESTINATION lib${LIB_SUFFIX} )
#              install( TARGETS      kite.bin       DESTINATION bin )
               install( FILES        ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis2/kite ) 

   
