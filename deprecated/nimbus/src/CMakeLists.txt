
#              include ( ${QT_USE_FILE} )

   include_directories ( ${NIMBUS_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR}
                         ${CONFIGURATION_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )
                   set ( includes    nimbus/Configuration.h
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
                   set ( pyIncludes  nimbus/PyNimbusEngine.h
                       )
                   set ( cpps        Configuration.cpp
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
                   set ( pyCpps      PyNimbus.cpp
                                     PyNimbusEngine.cpp
                       )
      

           add_library ( nimbus    ${cpps} ${pyCpps} )
 set_target_properties ( nimbus    PROPERTIES VERSION 1.0 SOVERSION 1 )
#target_link_libraries ( nimbus    ${CORIOLIS_LIBRARIES}  
#                                  ${HURRICANE_PYTHON_LIBRARIES}
#                                  ${HURRICANE_GRAPHICAL_LIBRARIES}
#                                  ${HURRICANE_LIBRARIES}
#                                  ${CONFIGURATION_LIBRARY}
#                                  ${CIF_LIBRARY}
#                                  ${AGDS_LIBRARY}
#                                  ${LEFDEF_LIBRARIES}
#                                  ${OA_LIBRARIES}
#                                  ${QtX_LIBRARIES}
#                                  ${Boost_LIBRARIES}
#                                  ${LIBXML2_LIBRARIES}
#                                  ${Python_LIBRARIES} -lutil
#                      )

           add_library ( pyNimbus  MODULE ${pyCpps} )
 set_target_properties ( pyNimbus  PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                              PREFIX        ""
                                              OUTPUT_NAME   "Nimbus"
                       )
#target_link_libraries ( pyNimbus  nimbus 
#                                  ${CORIOLIS_PYTHON_LIBRARIES}
#                      )

               install ( TARGETS nimbus         DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS pyNimbus       DESTINATION ${PYTHON_SITE_PACKAGES} )
               install ( FILES   ${includes} 
                                 ${pyIncludes}  DESTINATION include/coriolis2/nimbus ) 
   
