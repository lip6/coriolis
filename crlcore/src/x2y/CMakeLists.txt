   
               include ( ${QT_USE_FILE} )
   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${HURRICANE_INCLUDE_DIR}
                         ${CRLCORE_SOURCE_DIR}/src/ccore
                       )

		   set ( cpps        x2y.cpp
                       )
      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )
 
        add_executable ( cx2y        ${cpps} )
 target_link_libraries ( cx2y        crlcore
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QT_LIBRARIES}
                                     ${Boost_LIBRARIES} )
               install ( TARGETS     cx2y DESTINATION bin )
