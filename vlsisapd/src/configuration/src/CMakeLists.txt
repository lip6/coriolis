
               include ( ${QT_USE_FILE} )
   include_directories ( ${VLSISAPD_SOURCE_DIR}/src/configuration/src
                         ${Boost_INCLUDE_DIRS}
                         ${LIBXML2_INCLUDE_DIR}
                       )

                   set ( mocIncludes   vlsisapd/configuration/ConfigurationWidget.h
                                       vlsisapd/configuration/ParameterWidget.h
                                       vlsisapd/configuration/ConfTabWidget.h
                                       vlsisapd/configuration/ConfEditorWidget.h
                       )
		   set ( includes      vlsisapd/configuration/Parameter.h
                                       vlsisapd/configuration/LayoutDescription.h
                                       vlsisapd/configuration/Configuration.h
                       )
		   set ( cpps          Parameter.cpp
                                       LayoutDescription.cpp
                                       Configuration.cpp
                                       ParameterWidget.cpp
                                       ConfTabWidget.cpp
                                       ConfigurationWidget.cpp
                                       ConfEditorWidget.cpp
                       )
                   set ( editorcpp     ConfEditorMain.cpp )

          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )

           add_library ( configuration ${cpps} ${mocCpps} )
 target_link_libraries ( configuration ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                       )
        add_executable ( vlsisapd-conf-editor ${editorcpp} )
 target_link_libraries ( vlsisapd-conf-editor configuration ${QT_LIBRARIES}
                                                            ${Boost_LIBRARIES}
                                                            ${PYTHON_LIBRARIES}
                                                            ${LIBXML2_LIBRARIES}
                       )

               install ( TARGETS       configuration DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS       vlsisapd-conf-editor DESTINATION bin )
               install ( FILES         ${includes} ${mocIncludes}
                                       DESTINATION include/vlsisapd/configuration ) 

