
       include_directories ( ${CRLCORE_SOURCE_DIR}/src/fonts
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
                           )

           add_definitions ( -DCORIOLIS_TOP="${CORIOLIS_TOP}"
                             -DSYS_CONF_DIR="${SYS_CONF_DIR}"
                             -DPYTHON_SITE_PACKAGES="${PYTHON_SITE_PACKAGES}"
                           )

                       set ( sources   PyCRL.cpp
                                       PyCatalog.cpp
                                       PyCatalogState.cpp
                                       PyAllianceFramework.cpp
                                       PyToolBox.cpp
                           )	    
                       set ( includes  crlcore/PyCatalog.h
                                       crlcore/PyCatalogState.h
                                       crlcore/PyAllianceFramework.h
                                       crlcore/PyToolBox.h
                           )


               add_library ( pycrlcore  ${sources} )
     target_link_libraries ( pycrlcore  crlcore
                                        ${HURRICANE_PYTHON_LIBRARIES}
                                        ${PYTHON_LIBRARIES} -lutil
                           )
               add_library ( CRL MODULE ${sources} ) 
     target_link_libraries ( CRL pycrlcore )
     set_target_properties ( CRL PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                            PREFIX        ""
                           )

                   install ( FILES   ${includes} DESTINATION include/coriolis2/crlcore )
                   install ( TARGETS pycrlcore   DESTINATION lib${LIB_SUFFIX} )
                   install ( TARGETS CRL         DESTINATION ${PYTHON_SITE_PACKAGES} )
