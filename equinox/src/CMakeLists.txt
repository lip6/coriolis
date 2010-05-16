
               include ( ${QT_USE_FILE} )

   include_directories ( ${EQUINOX_SOURCE_DIR}/src
                         ${EQUINOX_SOURCE_DIR}/src/intervalTree/src
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                       )


                   set ( includes   equinox/EquinoxFilters.h
			            equinox/EquinoxCollections.h
                                    equinox/Equi.h                   equinox/Equis.h
                                    equinox/EquinoxEngine.h
                                    equinox/Strategy.h
                                    equinox/Tile.h                   equinox/Tiles.h
                                    equinox/UnionFind.h
				    equinox/SweepLine.h             equinox/TileSweepLine.h
                       )
                   set ( mocIncludes   equinox/GraphicEquinoxEngine.h )

		   set ( cpps          EquinoxFilters.cpp
                                       EquinoxCollections.cpp
                                       Equi.cpp                         
		                       TileSweepLine.cpp
                                       Tile.cpp
                                       Strategy.cpp
                                       EquinoxEngine.cpp
                                       UnionFind.cpp
                                       GraphicEquinoxEngine.cpp 
                       )

                   set ( intervalTreeIncludes intervalTree/src/equinox/Interval.h
		                              intervalTree/src/equinox/IntervalTree.h )
                   set ( intervalTreeCpps     intervalTree/src/IntervalTree.cpp )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )

           add_library ( equinox        ${cpps} ${mocCpps} )
 target_link_libraries ( equinox        intervalTree
                                        ${CORIOLIS_LIBRARIES}
                                        ${HURRICANE_LIBRARIES}
                                        ${HURRICANE_GRAPHICAL_LIBRARIES}
                                        ${QT_LIBRARIES}
                                        ${LEFDEF_LIBRARIES}
                                        ${OA_LIBRARIES}
                       )
           add_library (intervalTree    ${intervalTreeCpps} )
 target_link_libraries (intervalTree    ${CORIOLIS_LIBRARIES}
                                        ${HURRICANE_LIBRARIES} )
               install ( TARGETS        equinox intervalTree DESTINATION lib)
               install ( FILES          ${includes}
                                        ${mocIncludes} 
                                        ${intervalTreeIncludes} DESTINATION include/coriolis/equinox ) 


