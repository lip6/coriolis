
               include ( ${QT_USE_FILE} )

   include_directories ( ${KITE_SOURCE_DIR}/src
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                         ${Boost_INCLUDE_DIRS}
                       )
                   set ( includes      kite/TrackSegmentCost.h
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
                                       kite/Session.h
                                       kite/RoutingEvent.h
                                       kite/RoutingEventQueue.h
                                       kite/RoutingEventHistory.h
                                       kite/RoutingEventLoop.h
                                       kite/RoutingPlane.h
                                       kite/NegociateWindow.h
                                       kite/Configuration.h
                                       kite/KiteEngine.h
                                       kite/GraphicKiteEngine.h
                       )
		   set ( mocIncludes   kite/GraphicKiteEngine.h )
                   set ( cpps          TrackSegmentCost.cpp
                                       TrackCost.cpp
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
                                       Session.cpp
                                       RoutingEvent.cpp
                                       RoutingEventQueue.cpp
                                       RoutingEventHistory.cpp
                                       RoutingEventLoop.cpp
                                       RoutingPlane.cpp
                                       BuildPowerRails.cpp
                                       ProtectRoutingPads.cpp
                                       PreProcess.cpp
                                       NegociateWindow.cpp
                                       Configuration.cpp
                                       KiteEngine.cpp
                                       GraphicKiteEngine.cpp
                       )
                   set ( kitecpps      KiteMain.cpp )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( kite          ${cpps} ${mocCpps} )
 set_target_properties ( kite          PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( kite          ${KATABATIC_LIBRARIES}
                                       ${KNIK_LIBRARIES}
                                       ${NIMBUS_LIBRARIES}
                                       ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${BOOKSHELF_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${PYTHON_LIBRARIES} -lutil
                       )

        add_executable ( kite-text     ${kitecpps} )
 target_link_libraries ( kite-text     kite )
               install ( TARGETS       kite      DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS       kite-text DESTINATION bin )

               install ( FILES         ${includes}
                                       ${mocIncludes} DESTINATION include/coriolis2/kite ) 
   
