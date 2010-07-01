   
               include ( ${QT_USE_FILE} )
   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                       )

		   set ( cpps        x2y.cpp
                       )
      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )
 
        add_executable ( cx2y        ${cpps} )
 target_link_libraries ( cx2y        crlcore
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${CONFIGURATION_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${OA_LIBRARIES}
                                     ${QT_LIBRARIES}
                                     ${LEFDEF_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                     ${PYTHON_LIBRARIES}
                                     -lutil
                       )
               install ( TARGETS     cx2y DESTINATION bin )
