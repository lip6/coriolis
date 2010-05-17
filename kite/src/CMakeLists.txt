
               include ( ${QT_USE_FILE} )

   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${KITE_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                       )
                   set ( includes      kite/TrackSegmentCost.h
                                       kite/TrackCost.h
                                       kite/DataNegociate.h
                                       kite/TrackElement.h       kite/TrackElements.h
                                       kite/TrackSegment.h
                                       kite/TrackBlockage.h
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
                                       kite/GCell.h
                                       kite/GCellGrid.h
                                       kite/GCellRoutingSet.h
                                       kite/RoutingPlane.h
                                       kite/NegociateWindow.h
                                       kite/Configuration.h
                                       kite/KiteEngine.h
                                       kite/GraphicKiteEngine.h
                                       kite/ConfigurationWidget.h
                       )
		   set ( mocIncludes   kite/GraphicKiteEngine.h
                                       kite/ConfigurationWidget.h
                       )
                   set ( cpps          TrackSegmentCost.cpp
                                       TrackCost.cpp
                                       DataNegociate.cpp
                                       TrackElement.cpp
                                       TrackElements.cpp
                                       TrackSegment.cpp
                                       TrackBlockage.cpp
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
                                       GCell.cpp
                                       GCellGrid.cpp
                                       GCellRoutingSet.cpp
                                       RoutingPlane.cpp
                                       BuildBlockages.cpp
                                       BuildPowerRails.cpp
                                       PreProcess.cpp
                                       NegociateWindow.cpp
                                       Configuration.cpp
                                       KiteEngine.cpp
                                       GraphicKiteEngine.cpp
                                       ConfigurationWidget.cpp
                       )
                   set ( kitecpps      KiteMain.cpp )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( kite             ${cpps} ${mocCpps} )
 target_link_libraries ( kite             ${KATABATIC_LIBRARIES}
                                          ${KNIK_LIBRARIES}
                                          ${CORIOLIS_LIBRARIES}
                                          ${HURRICANE_LIBRARIES}
                                          ${HURRICANE_GRAPHICAL_LIBRARIES}
                                          ${CIF_LIBRARY}
                                          ${AGDS_LIBRARY}
                                          ${QT_LIBRARIES}
                                          ${LEFDEF_LIBRARIES}
                                          ${OA_LIBRARIES}
                       )		  
        add_executable ( kite-text        ${kitecpps} )
 target_link_libraries ( kite-text        kite
                                          ${KATABATIC_LIBRARIES}
                                          ${KNIK_LIBRARIES}
                                          ${CORIOLIS_LIBRARIES}
                                          ${HURRICANE_LIBRARIES}
                                          ${HURRICANE_GRAPHICAL_LIBRARIES}
                                          ${CIF_LIBRARY}
                                          ${AGDS_LIBRARY}
                                          ${QT_LIBRARIES}
                                          ${LEFDEF_LIBRARIES}
                                          ${OA_LIBRARIES}
                                          ${Boost_LIBRARIES}
                       )
               install ( TARGETS          kite      DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS          kite-text DESTINATION bin )

               install ( FILES       ${includes}
                                     ${mocIncludes} DESTINATION include/coriolis/kite ) 
   
