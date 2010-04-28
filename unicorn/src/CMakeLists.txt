
               include ( ${QT_USE_FILE} )
   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                       )

                   set ( mocincludes   unicorn/UnicornGui.h
                                       unicorn/COpenCellDialog.h
                       )
		   set ( cpps          COpenCellDialog.cpp
                                       UnicornGui.cpp
                       )
                   set ( cgtcpp        CgtMain.cpp )
 
          qt4_wrap_cpp ( MOCcpps       ${mocincludes} )
     qt4_add_resources ( RCC_SRCS      Unicorn.qrc )

           add_library ( unicorn        ${cpps} ${MOCcpps} )
 target_link_libraries ( unicorn        ${SOLSTICE_GRAPHICAL_LIBRARIES}
                                        ${SOLSTICE_LIBRARIES}
                                        ${EQUINOX_GRAPHICAL_LIBRARIES}
                                        ${EQUINOX_LIBRARIES}
                                        ${KITE_GRAPHICAL_LIBRARIES}
                                        ${KITE_LIBRARIES}
                                        ${KATABATIC_GRAPHICAL_LIBRARIES}
                                        ${KATABATIC_LIBRARIES}
                                        ${KNIK_GRAPHICAL_LIBRARIES}
                                        ${KNIK_LIBRARIES}
                                        ${CORIOLIS_LIBRARIES}
                                        ${HURRICANE_GRAPHICAL_LIBRARIES}
                                        ${HURRICANE_LIBRARIES}
                                        ${AGDS_LIBRARY}
                                        ${CIF_LIBRARY}
                                        ${LEFDEF_LIBRARIES}
                                        ${OA_LIBRARIES}
                                        ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                       )
        add_executable ( cgt            ${cgtcpp} )
 target_link_libraries ( cgt            unicorn
                                        ${SOLSTICE_GRAPHICAL_LIBRARIES}
                                        ${SOLSTICE_LIBRARIES}
                                        ${EQUINOX_GRAPHICAL_LIBRARIES}
                                        ${EQUINOX_LIBRARIES}
                                        ${KITE_GRAPHICAL_LIBRARIES}
                                        ${KITE_LIBRARIES}
                                        ${KATABATIC_GRAPHICAL_LIBRARIES}
                                        ${KATABATIC_LIBRARIES}
                                        ${KNIK_GRAPHICAL_LIBRARIES}
                                        ${KNIK_LIBRARIES}
                                        ${CORIOLIS_LIBRARIES}
                                        ${HURRICANE_LIBRARIES}
                                        ${HURRICANE_GRAPHICAL_LIBRARIES}
                                        ${OA_LIBRARIES}
                                        ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                       )
               install ( TARGETS        unicorn DESTINATION /lib)
               install ( TARGETS        cgt     DESTINATION /bin )
               install ( FILES          ${includes}
                                        ${mocIncludes} DESTINATION /include/coriolis/unicorn ) 

