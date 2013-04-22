
               include ( ${QT_USE_FILE} )

   include_directories ( ${KNIK_SOURCE_DIR}/src
                         ${KNIK_SOURCE_DIR}/src/flute-2.4/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${UTILITIES_INCLUDE_DIR} 
                         ${Boost_INCLUDE_DIRS}
                       )
                   set ( includes      knik/Configuration.h
                                       knik/Vertex.h           knik/Vertexes.h
                                       knik/Tuple.h
                                       knik/STuple.h
                                       knik/VTuple.h
                                       knik/Edge.h             knik/Edges.h
                                       knik/HEdge.h
                                       knik/VEdge.h
                                       knik/MatrixVertex.h
                                       knik/RoutingGrid.h
                                       knik/SlicingTree.h
                                       knik/SlicingTreeNode.h
                                       knik/Graph.h
                                       knik/NetExtension.h
                                       knik/KnikEngine.h
                       )
		   set ( mocIncludes   knik/GraphicKnikEngine.h )
                   set ( cpps          Configuration.cpp
                                       Vertex.cpp
                                       Edge.cpp
                                       HEdge.cpp
                                       VEdge.cpp
                                       MatrixVertex.cpp
                                       Graph.cpp
                                       SlicingTree.cpp
                                       NetExtension.cpp
                                       LoadSolution.cpp
                                       KnikEngine.cpp
                                       GraphicKnikEngine.cpp
                       )
                   set ( fluteIncludes flute-2.4/src/knik/flute.h )
                   set ( fluteCpps     flute-2.4/src/flute.cpp )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( flute         ${fluteCpps} )
 set_target_properties ( flute         PROPERTIES VERSION 2.4 SOVERSION 2 )
 target_link_libraries ( flute         ${HURRICANE_LIBRARIES}
                                       ${CORIOLIS_LIBRARIES}
                       )	    
           add_library ( knik          ${cpps} ${mocCpps} )
 set_target_properties ( knik          PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( knik          flute
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${UTILITIES_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${PYTHON_LIBRARIES} -lutil
                       )	    
				    
               install ( TARGETS       knik flute DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes}
                                       ${mocIncludes}
                                       ${fluteIncludes} DESTINATION include/coriolis2/knik ) 
               install ( FILES         flute-2.4/etc/POST9.dat
                                       flute-2.4/etc/POWV9.dat DESTINATION share/coriolis2/flute-2.4 ) 
