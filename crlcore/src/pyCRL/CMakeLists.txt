# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/pyCRL>" -*-

   include_directories( ${CRLCORE_SOURCE_DIR}/src/fonts
                        ${CRLCORE_SOURCE_DIR}/src/ccore
                        ${CRLCORE_SOURCE_DIR}/src/ccore/properties
                        ${CRLCORE_SOURCE_DIR}/src/ccore/bookshelf
                        ${CRLCORE_SOURCE_DIR}/src/ccore/cspice
                        ${CRLCORE_SOURCE_DIR}/src/ccore/lefdef
                        ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/ap
                        ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/vst
                        ${CRLCORE_SOURCE_DIR}/src/ccore/agds
                        ${CRLCORE_SOURCE_DIR}/src/ccore/cif
                        ${CRLCORE_SOURCE_DIR}/src/ccore/spice
                        ${CRLCORE_SOURCE_DIR}/src/ccore/liberty
                        ${CRLCORE_SOURCE_DIR}/src/ccore/toolbox
                        ${CRLCORE_SOURCE_DIR}/src/pyCRL
                        ${HURRICANE_INCLUDE_DIR}
                        ${CIF_INCLUDE_DIR}
                        ${CONFIGURATION_INCLUDE_DIR}
                        ${PYTHON_INCLUDE_PATH}
                        ${Boost_INCLUDE_DIR}
                      )

       add_definitions( -DCORIOLIS_TOP="${CORIOLIS_TOP}"
                        -DSYS_CONF_DIR="${SYS_CONF_DIR}"
                        -DPYTHON_SITE_PACKAGES="${PYTHON_SITE_PACKAGES}"
                      )

                   set( pyCpps      PyCRL.cpp
                                    PyBanner.cpp
                                    PyCatalog.cpp
                                    PyCatalogState.cpp
                                    PyEnvironment.cpp
                                    PyCellGauge.cpp
                                    PyRoutingGauge.cpp
                                    PyRoutingLayerGauge.cpp
                                    PyAllianceFramework.cpp
                                    PyToolBox.cpp
                                    PyToolEngine.cpp
                                    PyToolEngineCollection.cpp
                                    PyGraphicToolEngine.cpp
                                    PyAcmSigda.cpp
                                    PyIspd05.cpp
                                    PyBlif.cpp
                      )	    
                   set( pyIncludes  crlcore/PyBanner.h
                                    crlcore/PyCatalog.h
                                    crlcore/PyCatalogState.h
                                    crlcore/PyEnvironment.h
                                    crlcore/PyCellGauge.h
                                    crlcore/PyRoutingGauge.h
                                    crlcore/PyRoutingLayerGauge.h
                                    crlcore/PyAllianceFramework.h
                                    crlcore/PyToolBox.h
                                    crlcore/PyToolEngine.h
                                    crlcore/PyToolEngineCollection.h
                                    crlcore/PyGraphicToolEngine.h
                                    crlcore/PyAcmSigda.h
                                    crlcore/PyIspd05.h
                                    crlcore/PyBlif.h
                      )	         
                   set( depLibs     crlcore
                                    ${HURRICANE_PYTHON_LIBRARIES}
                                    ${PYTHON_LIBRARIES}
                                    -lutil
                      )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "pycrlcore;1.0;1"
                        CRL
                        "${depLibs}"
                        include/coriolis2/crlcore
                      )
