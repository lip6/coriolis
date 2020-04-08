
   include_directories( ${HURRICANE_SOURCE_DIR}/src/isobar
                        ${HURRICANE_SOURCE_DIR}/src/hurricane
                        ${HURRICANE_SOURCE_DIR}/src/configuration
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${PYTHON_INCLUDE_PATH}
                        ${LIBXML2_INCLUDE_DIR}
                      )

                   set( mocIncludes     hurricane/configuration/ConfigurationDialog.h
                                        hurricane/configuration/ParameterWidget.h
                                        hurricane/configuration/ConfTabWidget.h
                                        hurricane/configuration/LogWidget.h
                                        hurricane/configuration/ConfigurationWidget.h
                                        hurricane/configuration/ConfEditorWidget.h
                      )             
                   set( includes        hurricane/configuration/Parameter.h
                                        hurricane/configuration/LayoutDescription.h
                                        hurricane/configuration/Configuration.h
                      )             
                   set( pyIncludes      hurricane/configuration/PyHurricane2.h
                                        hurricane/configuration/PyParameter.h
                                        hurricane/configuration/PyConfiguration.h
                      )
                   set( cpps            Parameter.cpp
                                        LayoutDescription.cpp
                                        Configuration.cpp
                                        ParameterWidget.cpp
                                        ConfTabWidget.cpp
                                        LogWidget.cpp
                                        ConfigurationWidget.cpp
                                        ConfigurationDialog.cpp
                                        ConfEditorWidget.cpp
                      )
                   set( pyCpps          PyHurricane2.cpp
                                        PyParameter.cpp
                                        PyConfiguration.cpp
                                        PyCfg.cpp
                      )
                        
          qtX_wrap_cpp( mocCpps         ${mocIncludes} )
     qtX_add_resources( RCC_SRCS        Configuration.qrc )
                        
           add_library( configuration2  ${cpps} ${mocCpps} ${RCC_SRCS} )
 set_target_properties( configuration2  PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( configuration2  isobar
                                        hurricane
                                        ${QtX_LIBRARIES}
                                        ${PYTHON_LIBRARIES}
                      )
     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "cfg2;1.0;1"
                        Cfg2
                        "configuration2;${depLibs}"
                        include/coriolis2/hurricane/configuration
                      )

               install( TARGETS         configuration2 DESTINATION lib${LIB_SUFFIX} )
               install( FILES           ${includes} ${mocIncludes}
                                        DESTINATION include/hurricane/configuration ) 

