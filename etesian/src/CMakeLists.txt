# -*- explicit-buffer-name: "CMakeLists.txt<etesian/src>" -*-

#              include ( ${QT_USE_FILE} )

   include_directories ( ${ETESIAN_SOURCE_DIR}/src
                         ${COLOQUINTE_INCLUDE_DIR} 
                         ${EIGEN3_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                         ${QtX_INCLUDE_DIRS}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )
                   set ( includes     #etesian/Constants.h
                                      #etesian/TrackCost.h
                                      #etesian/DataNegociate.h
                                      #etesian/TrackElement.h       etesian/TrackElements.h
                                      #etesian/TrackSegment.h
                                      #etesian/TrackFixedSegment.h
                                      #etesian/TrackMarker.h
                                      #etesian/Track.h
                                      #etesian/Tracks.h
                                      #etesian/HorizontalTrack.h
                                      #etesian/VerticalTrack.h
                                      #etesian/RoutingPlane.h
                                      #etesian/Session.h
				      #etesian/Manipulator.h
				      #etesian/SegmentFsm.h
                                      #etesian/RoutingEvent.h
                                      #etesian/RoutingEventQueue.h
                                      #etesian/RoutingEventHistory.h
                                      #etesian/RoutingEventLoop.h
                                      #etesian/NegociateWindow.h
                                       etesian/Configuration.h
                                       etesian/EtesianEngine.h
                                       etesian/GraphicEtesianEngine.h
                       )	       
                   set ( pyIncludes    etesian/PyEtesianEngine.h
                                       etesian/PyGraphicEtesianEngine.h
                       )
		   set ( mocIncludes   etesian/GraphicEtesianEngine.h )
                   set ( cpps         #TrackCost.cpp
                                      #DataNegociate.cpp
                                      #TrackElement.cpp
                                      #TrackElements.cpp
                                      #TrackSegment.cpp
                                      #TrackFixedSegment.cpp
                                      #TrackMarker.cpp
                                      #Track.cpp
                                      #Tracks.cpp
                                      #HorizontalTrack.cpp
                                      #VerticalTrack.cpp
                                      #RoutingPlane.cpp
                                      #Session.cpp
				      #Manipulator.cpp
				      #SegmentFsm.cpp
                                      #RoutingEvent.cpp
                                      #RoutingEventQueue.cpp
                                      #RoutingEventHistory.cpp
                                      #RoutingEventLoop.cpp
                                      #NegociateWindow.cpp
                                      #BuildPowerRails.cpp
                                      #ProtectRoutingPads.cpp
                                      #PreProcess.cpp
                                       Configuration.cpp
                                       EtesianEngine.cpp
                                       GraphicEtesianEngine.cpp
                       )
                   set ( pyCpps        PyEtesian.cpp
                                       PyEtesianEngine.cpp
                                       PyGraphicEtesianEngine.cpp
                       )
#                  set ( etesiancpps   EtesianMain.cpp )
          qt5_wrap_cpp ( mocCpps       ${mocIncludes} )

           add_library ( etesian       ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties ( etesian       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( etesian       ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${BOOKSHELF_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${UTILITIES_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${COLOQUINTE_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QtX_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${PYTHON_LIBRARIES} -lutil
                                       ${LIBEXECINFO_LIBRARIES}
                       )

           add_library ( pyEtesian     MODULE ${pyCpps} )
 set_target_properties ( pyEtesian     PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                                  PREFIX        ""
                                                  OUTPUT_NAME   "Etesian"
                       )

#       add_executable ( etesian.bin   ${etesiancpps} )
#target_link_libraries ( etesian.bin   etesian )
 target_link_libraries ( pyEtesian     etesian
                                       ${CORIOLIS_PYTHON_LIBRARIES}
                       )
               install ( TARGETS       etesian           DESTINATION lib${LIB_SUFFIX} )
#              install ( TARGETS       etesian.bin       DESTINATION bin )
               install ( TARGETS       pyEtesian         DESTINATION ${PYTHON_SITE_PACKAGES} )

               install ( FILES         ${includes}
                                       ${mocIncludes}
                                       ${pyIncludes}   DESTINATION include/coriolis2/etesian ) 
   
