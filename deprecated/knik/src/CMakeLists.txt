# -*- explicit-buffer-name: "CMakeLists.txt<knik/src>" -*-

#              include ( ${QT_USE_FILE} )
   include_directories ( ${KNIK_SOURCE_DIR}/src
                         ${KNIK_SOURCE_DIR}/src/flute-3.1/src
                         ${FLUTE_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${UTILITIES_INCLUDE_DIR} 
                         ${QtX_INCLUDE_DIRS}
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
          qtX_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( knik          ${cpps} ${mocCpps} )
 set_target_properties ( knik          PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( knik          ${FLUTE_LIBRARY}
                                       ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${UTILITIES_LIBRARY}
                                       ${FLUTE_LIBRARIES}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QtX_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                        -lutil
                       )	    
				    
               install ( TARGETS       knik DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes}
                                       ${mocIncludes} DESTINATION include/coriolis2/knik ) 
