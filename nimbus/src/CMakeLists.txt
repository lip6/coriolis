
               include ( ${QT_USE_FILE} )

   include_directories ( ${NIMBUS_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR}
                         ${CONFIGURATION_INCLUDE_DIR}
                       )
                   set ( includes nimbus/Configuration.h
                                  nimbus/SplitterContact.h nimbus/SplitterContacts.h
                                  nimbus/Splitter.h        nimbus/Splitters.h
                                  nimbus/Fence.h           nimbus/Fences.h
                                  nimbus/HFence.h
                                  nimbus/VFence.h
                                  nimbus/FrontLine.h
                                  nimbus/FenceProperty.h
                                  nimbus/StepProperty.h
                                  nimbus/GCell.h           nimbus/GCells.h
                                  nimbus/Grid.h
                                  nimbus/NimbusEngine.h
                                  nimbus/RoutTools.h
                       )
                   set ( cpps     Configuration.cpp
                                  SplitterContact.cpp
                                  Splitter.cpp
                                  Fence.cpp
                                  FenceProperty.cpp
                                  HFence.cpp
                                  VFence.cpp
                                  FrontLine.cpp
                                  GCell.cpp
                                  Grid.cpp
                                  StepProperty.cpp
                                  NimbusEngine.cpp
                                  RoutTools.cpp
                       )
      

           add_library ( nimbus ${cpps} )
 target_link_libraries ( nimbus ${HURRICANE_LIBRARIES}
                                ${HURRICANE_GRAPHICAL_LIBRARIES}
                                ${LEFDEF_LIBRARIES}
                                ${OA_LIBRARIES}
                                ${QT_LIBRARIES}
                       )
               install ( TARGETS nimbus DESTINATION lib${LIB_SUFFIX} )
               install ( FILES   ${includes} DESTINATION include/coriolis2/nimbus ) 
   
