
   message( "Python_INCLUDE_DIRS=${Python_INCLUDE_DIRS}" )
   include_directories( ${HURRICANE_SOURCE_DIR}/src/utilities
                        ${HURRICANE_SOURCE_DIR}/src/hurricane
                        ${HURRICANE_SOURCE_DIR}/src/configuration
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${Python_INCLUDE_DIRS}
                        ${LIBXML2_INCLUDE_DIR}
                      )

                   set( mocIncludes          hurricane/configuration/ConfigurationDialog.h
                                             hurricane/configuration/ParameterWidget.h
                                             hurricane/configuration/ConfTabWidget.h
                                             hurricane/configuration/LogWidget.h
                                             hurricane/configuration/ConfigurationWidget.h
                                             hurricane/configuration/ConfEditorWidget.h
                      )             	     
                   set( includes             hurricane/configuration/Parameter.h
                                             hurricane/configuration/LayoutDescription.h
                                             hurricane/configuration/Configuration.h
                      )             	     
                   set( pyTypeIncludes       hurricane/configuration/ProxyProperty.h
                                             hurricane/configuration/PyTypeManager.h
                                             hurricane/configuration/PyVector.h
                                             hurricane/configuration/PyMap.h
                      )			     
                   set( pyCfgIncludes        hurricane/configuration/PyParameter.h
                                             hurricane/configuration/PyConfiguration.h
                                             hurricane/configuration/PyLayoutDescription.h
                      )
                   set( pyHurricane3Includes hurricane/configuration/PyDbU.h
		                             hurricane/configuration/PyPoint.h
		                             hurricane/configuration/PyBox.h
		                             hurricane/configuration/PyName.h
		                             hurricane/configuration/PyDataBase.h
		                             hurricane/configuration/PyTechnology.h
		                             hurricane/configuration/PyLayerMask.h
		                             hurricane/configuration/PyLayer.h
		                             hurricane/configuration/PyMaterial.h
		                             hurricane/configuration/PyBasicLayer.h
		                             hurricane/configuration/PyViaLayer.h
		                             hurricane/configuration/PyRegularLayer.h
		                             hurricane/configuration/PyDiffusionLayer.h
		                             hurricane/configuration/PyTransistorLayer.h
                      )
                   set( cpps                 Parameter.cpp
                                             LayoutDescription.cpp
                                             Configuration.cpp
                                             ParameterWidget.cpp
                                             ConfTabWidget.cpp
                                             LogWidget.cpp
                                             ConfigurationWidget.cpp
                                             ConfigurationDialog.cpp
                                             ConfEditorWidget.cpp
                      )			     
                   set( pyTypeCpps           ProxyProperty.cpp
                                             PyTypeManager.cpp
                      )			     
                   set( pyCfgCpps            PyParameter.cpp
                                             PyConfiguration.cpp
                                             PyLayoutDescription.cpp
                                             PyCfg.cpp
                      )
                   set( pyHurricane3Cpps     PyDbU.cpp
                                             PyPoint.cpp
                                             PyBox.cpp
                                             PyDataBase.cpp
                                             PyTechnology.cpp
                                             PyLayerMask.cpp
                                             PyLayer.cpp
                                             PyMaterial.cpp
                                             PyBasicLayer.cpp
                                             PyViaLayer.cpp
                                             PyRegularLayer.cpp
                                             PyDiffusionLayer.cpp
                                             PyTransistorLayer.cpp
                                             PyHurricane3.cpp
                      )
                        
          qtX_wrap_cpp( mocCpps        ${mocIncludes} )
     qtX_add_resources( RCC_SRCS       Configuration.qrc )
                        
           add_library( configuration  ${cpps} ${mocCpps} ${RCC_SRCS} )
 set_target_properties( configuration  PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( configuration  hurricane
                                       ${QtX_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                      )
                        
           add_library( pytypemanager  ${pyTypeCpps} )
 set_target_properties( pytypemanager  PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( pytypemanager  hurricane
                                       ${PYTHON_LIBRARIES}
                      )

    add_python_module3( "${pyCfgCpps}"
                        "${pyCfgIncludes}"
                        Cfg
                        "pytypemanager;configuration"
                        include/coriolis2/hurricane/configuration
                      )

    add_python_module3( "${pyHurricane3Cpps}"
                        "${pyHurricane3Includes}"
                        Hurricane3
                        "pytypemanager;configuration"
                        include/coriolis2/hurricane/configuration
                      )

               install( TARGETS configuration DESTINATION lib${LIB_SUFFIX} )
               install( TARGETS pytypemanager DESTINATION lib${LIB_SUFFIX} )
               install( FILES   ${pyTypeIncludes}
                                ${pyCfgIncludes}
                                ${pyHurricane3Includes}
                                ${includes} ${mocIncludes}
                                DESTINATION include/coriolis2/hurricane/configuration ) 

