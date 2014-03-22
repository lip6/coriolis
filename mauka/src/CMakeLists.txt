
#              include ( ${QT_USE_FILE} )

   include_directories ( ${MAUKA_SOURCE_DIR}/src
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                         ${QtX_INCLUDE_DIRS}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )
                   set ( includes      mauka/Configuration.h
                                       mauka/FeedCells.h
                                       mauka/BBPlacer.h
                                       mauka/Bin.h
                                       mauka/Container.h
                                       mauka/MaukaBox.h
                                       mauka/Move.h
                                       mauka/Row.h
                                       mauka/SimAnnealingPlacer.h
                                       mauka/SubRow.h
                                       mauka/Surface.h
                                       mauka/MaukaEngine.h
                       )
		   set ( mocIncludes   mauka/GraphicMaukaEngine.h )
                   set ( pyIncludes    mauka/PyMaukaEngine.h
                                       mauka/PyGraphicMaukaEngine.h
                       )
                   set ( cpps          Configuration.cpp
                                       BBPlacer.cpp
                                       FeedCells.cpp
                                       Bin.cpp
                                       Container.cpp
                                       MaukaBox.cpp
                                       Move.cpp
                                       Row.cpp
                                       SimAnnealingPlacer.cpp
                                       SubRow.cpp
                                       Surface.cpp
                                       InsertFeeds.cpp
                                       MaukaEngine.cpp
                                       GraphicMaukaEngine.cpp
                       )
                   set ( pyCpps        PyMauka.cpp
                                       PyMaukaEngine.cpp
                                       PyGraphicMaukaEngine.cpp
                       )
          qt5_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( mauka         ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties ( mauka         PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( mauka         ${METIS_LIBRARIES}
                                       ${NIMBUS_LIBRARIES}
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

           add_library ( pyMauka       MODULE ${pyCpps} )
 set_target_properties ( pyMauka       PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                                  PREFIX        ""
                                                  OUTPUT_NAME   "Mauka"
                       )
 target_link_libraries ( pyMauka       mauka
                                       ${CORIOLIS_PYTHON_LIBRARIES}
                       )

               install ( TARGETS       mauka           DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS       pyMauka         DESTINATION ${PYTHON_SITE_PACKAGES} )
               install ( FILES         ${includes}
                                       ${mocIncludes}
                                       ${pyIncludes}   DESTINATION include/coriolis2/mauka ) 
