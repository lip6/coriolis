
               include ( ${QT_USE_FILE} )
   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                       )

                   set ( mocincludes   IspdGui.h
                       )
                   set ( cpps          IspdGui.cpp
                                       IspdMain.cpp
                       )
 
          qt4_wrap_cpp ( MOCcpps     ${mocincludes} )

        add_executable ( ispd        ${cpps} ${MOCcpps} )
 target_link_libraries ( ispd        ${KATABATIC_GRAPHICAL_LIBRARIES}
                                     ${KATABATIC_LIBRARIES}
                                     ${KNIK_GRAPHICAL_LIBRARIES}
                                     ${KNIK_LIBRARIES}
                                     ${CORIOLIS_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${AGDS_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${OA_LIBRARIES}
                                     ${LEFDEF_LIBRARIES}
                                     ${QT_LIBRARIES}
                                     ${Boost_LIBRARIES} )
               install ( TARGETS     ispd DESTINATION bin )

