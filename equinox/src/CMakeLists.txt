
#              include ( ${QT_USE_FILE} )

   include_directories ( ${EQUINOX_SOURCE_DIR}/src
                         ${EQUINOX_SOURCE_DIR}/src/intervalTree/src
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${QtX_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                       )


                   set ( includes   equinox/EquinoxFilters.h
			            equinox/EquinoxCollections.h
                                    equinox/Equi.h                   equinox/Equis.h
                                    equinox/EquinoxEngine.h
                                    equinox/Strategy.h
                                    equinox/Tile.h                   equinox/Tiles.h
                                    equinox/UnionFind.h
				    equinox/SweepLine.h              equinox/TileSweepLine.h
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
          qtX_wrap_cpp ( mocCpps              ${mocIncludes} )

           add_library ( intervalTree   ${intervalTreeCpps} )
 set_target_properties ( intervalTree   PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( intervalTree   ${HURRICANE_LIBRARIES} )

           add_library ( equinox       ${cpps} ${mocCpps} )
 set_target_properties ( equinox       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( equinox       intervalTree
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

               install ( TARGETS        equinox intervalTree DESTINATION lib${LIB_SUFFIX} )
               install ( FILES          ${includes}
                                        ${mocIncludes} 
                                        ${intervalTreeIncludes} DESTINATION include/coriolis2/equinox ) 


