
               include ( ${QT_USE_FILE} )

   include_directories ( ${KNIK_SOURCE_DIR}/src
                         ${KNIK_SOURCE_DIR}/src/flute-2.4/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
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


           add_library ( knik         ${cpps} ${mocCpps} )
 target_link_libraries ( knik         flute
                                      ${CORIOLIS_LIBRARIES}
                                      ${HURRICANE_LIBRARIES}
                                      ${HURRICANE_GRAPHICAL_LIBRARIES}
                                      ${QT_LIBRARIES}
                                      ${LEFDEF_LIBRARIES}
                                      ${OA_LIBRARIES}
                       )
           add_library ( flute        ${fluteCpps} )
 target_link_libraries ( flute        ${CORIOLIS_LIBRARIES}
                                      ${HURRICANE_LIBRARIES} )
               install ( TARGETS      knik flute DESTINATION /lib)

               install ( FILES ${includes}
                               ${mocIncludes}
                               ${fluteIncludes} DESTINATION /include/coriolis/knik ) 
               install ( FILES flute-2.4/etc/POST9.dat
                               flute-2.4/etc/POWV9.dat DESTINATION /share/etc/flute-2.4 ) 
