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
                        ${Python_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIR}
                        ${QtX_INCLUDE_DIRS}
                      )
       set (LINK_LIBS ${HURRICANE_PYTHON_NEW_LIBRARIES}
                      ${HURRICANE_PYTHON_LIBRARIES}
                      ${HURRICANE_GRAPHICAL_LIBRARIES}
                      ${HURRICANE_LIBRARIES}
                      ${BOOKSHELF_LIBRARY}
                      ${OA_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
                       -lutil
                      )

       add_definitions( -DCORIOLIS_TOP="${CORIOLIS_TOP}"
                        -DSYS_CONF_DIR="${SYS_CONF_DIR}"
                        -DPYTHON_SITE_PACKAGES="${PYTHON_SITE_PACKAGES}"
                      )

                   set( pyConstCpps PyConstant.cpp )
                   set( pyCpps      PyCRL.cpp
                                    PySystem.cpp
                                    PyBanner.cpp
                                    PyCatalog.cpp
                                    PyCatalogState.cpp
                                    PyEnvironment.cpp
                                    PyAllianceLibrary.cpp
                                    PyCellGauge.cpp
                                    PyRoutingGauge.cpp
                                    PyRoutingLayerGauge.cpp
                                    PyAllianceFramework.cpp
                                    PyToolBox.cpp
                                    PyToolEngine.cpp
                                    PyToolEngineCollection.cpp
                                    PyGraphicToolEngine.cpp
                                    PyAcmSigda.cpp
                                   #PyIspd05.cpp
                                    PySpice.cpp
                                    PyBlif.cpp
                                    PyGds.cpp
                                    PyLefImport.cpp
                                    PyDefImport.cpp
                                    PyLefExport.cpp
                                    PyDefExport.cpp
                      )	    
                   set( pyIncludes  crlcore/PySystem.h
                                    crlcore/PyBanner.h
                                    crlcore/PyCatalog.h
                                    crlcore/PyCatalogState.h
                                    crlcore/PyEnvironment.h
                                    crlcore/PyAllianceLibrary.h
                                    crlcore/PyCellGauge.h
                                    crlcore/PyRoutingGauge.h
                                    crlcore/PyRoutingLayerGauge.h
                                    crlcore/PyAllianceFramework.h
                                    crlcore/PyToolBox.h
                                    crlcore/PyToolEngine.h
                                    crlcore/PyToolEngineCollection.h
                                    crlcore/PyGraphicToolEngine.h
                                    crlcore/PyAcmSigda.h
                                   #crlcore/PyIspd05.h
                                    crlcore/PySpice.h
                                    crlcore/PyBlif.h
                                    crlcore/PyGds.h
                                    crlcore/PyLefImport.h
                                    crlcore/PyDefImport.h
                                    crlcore/PyLefExport.h
                                    crlcore/PyDefExport.h
                      )	         
	      target_link_libraries ( crlcore ${LINK_LIBS} )

                   set( depLibs     crlcore
                                    ${HURRICANE_GRAPHICAL_LIBRARIES}
                                    ${HURRICANE_PYTHON_NEW_LIBRARIES}
                                    ${HURRICANE_PYTHON_LIBRARIES}
                                    ${HURRICANE_LIBRARIES}
                                    ${LEFDEF_LIBRARIES}
                                    ${QtX_LIBRARIES}
                                    ${Python_LIBRARIES}
                                    -lutil
                      )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "pycrlcore;1.0;1"
                        CRL
                        "${depLibs}"
                        include/coriolis2/crlcore
                      )

     add_python_module( "${pyConstCpps}"
                        "crlcore/PyRoutingLayerGauge.h"
                        "pycrlconst;1.0;1"
                        Constant
                        "${depLibs}"
                        include/coriolis2/crlcore
                      )

