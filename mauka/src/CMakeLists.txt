
               include ( ${QT_USE_FILE} )

   include_directories ( ${MAUKA_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CORIOLIS_INCLUDE_DIR} 
                       )
                   set ( includes      mauka/Configuration.h
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
		   set ( mocIncludes   mauka/ConfigurationWidget.h
                                       mauka/GraphicMaukaEngine.h )
                   set ( cpps          Configuration.cpp
                                       ConfigurationWidget.cpp
                                       BBPlacer.cpp
                                       Bin.cpp
                                       Container.cpp
                                       MaukaBox.cpp
                                       Move.cpp
                                       Row.cpp
                                       SimAnnealingPlacer.cpp
                                       SubRow.cpp
                                       Surface.cpp
                                       MaukaEngine.cpp
                                       GraphicMaukaEngine.cpp
                       )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( mauka         ${cpps} ${mocCpps} )
 target_link_libraries ( mauka         ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                       )
               install ( TARGETS       mauka                      DESTINATION lib${LIB_SUFFIX} )
               install ( FILES         ${includes} ${mocIncludes} DESTINATION include/coriolis2/mauka ) 
