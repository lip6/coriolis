
  # to work on sl5 64 bits : need to suppress boost_python in Boost_LIBRARIES
          set(Boost_LIBRARIES "")
  setup_boost(program_options filesystem regex)

               include ( ${QT_USE_FILE} )
   include_directories ( ${VLSISAPD_SOURCE_DIR}/src/configuration/src
                         ${VLSISAPD_SOURCE_DIR}/src/openChams/src
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                         ${LIBXML2_INCLUDE_DIR}
                       )

                   set ( mocIncludes     vlsisapd/configuration/FilePathEdit.h
                                         vlsisapd/configuration/ConfigurationDialog.h
                                         vlsisapd/configuration/ParameterWidget.h
                                         vlsisapd/configuration/ConfTabWidget.h
                                         vlsisapd/configuration/LogWidget.h
                                         vlsisapd/configuration/ConfigurationWidget.h
                                         vlsisapd/configuration/ConfEditorWidget.h
                       )	         
		   set ( includes        vlsisapd/configuration/Parameter.h
                                         vlsisapd/configuration/LayoutDescription.h
                                         vlsisapd/configuration/Configuration.h
                                         vlsisapd/configuration/BoostPythonStlWrappers.h
                       )	         
		   set ( cpps            Parameter.cpp
                                         LayoutDescription.cpp
                                         Configuration.cpp
                                         FilePathEdit.cpp
                                         ParameterWidget.cpp
                                         ConfTabWidget.cpp
                                         LogWidget.cpp
                                         ConfigurationWidget.cpp
                                         ConfigurationDialog.cpp
                                         ConfEditorWidget.cpp
                       )	         
                   set ( pycpps          PyConfiguration.cpp )
                   set ( editorcpp       ConfEditorMain.cpp )
				         
          qt4_wrap_cpp ( mocCpps         ${mocIncludes} )
     qt4_add_resources ( RCC_SRCS        Configuration.qrc )
				         
           add_library ( configuration   ${cpps} ${mocCpps} ${RCC_SRCS} )
 set_target_properties ( configuration   PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( configuration   ${QT_LIBRARIES}
                                         ${PYTHON_LIBRARIES}
                                         ${LIBXML2_LIBRARIES}
                                         ${Boost_LIBRARIES}
                       )

 if (Boost_FOUND)
           add_library ( pyConfiguration MODULE ${pycpps})
 set_target_properties ( pyConfiguration PROPERTIES
                                         OUTPUT_NAME "Cfg"
                                         PREFIX      ""
                       )
 target_link_libraries ( pyConfiguration configuration
                                         ${PYTHON_LIBRARIES}
                                         ${LIBXML2_LIBRARIES}
                                         ${Boost_LIBRARIES}
                       )
               install ( TARGETS         pyConfiguration DESTINATION ${PYTHON_SITE_PACKAGES} )
 endif(Boost_FOUND)

        add_executable ( vlsisapd-conf-editor ${editorcpp} )
 target_link_libraries ( vlsisapd-conf-editor configuration ${Boost_LIBRARIES} )

               install ( TARGETS         configuration DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS         vlsisapd-conf-editor DESTINATION bin )
               install ( FILES           ${includes} ${mocIncludes}
                                         DESTINATION include/vlsisapd/configuration ) 

