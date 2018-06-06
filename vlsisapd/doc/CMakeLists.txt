
 if(BUILD_DOC AND DOXYGEN_FOUND)
 add_custom_target( doc ALL
                    cd ${VLSISAPD_SOURCE_DIR}/doc && ${DOXYGEN_EXECUTABLE} doxyfile )
 endif()

 install( DIRECTORY html/  DESTINATION share/doc/coriolis2/en/html/vlsisapd )
 install( DIRECTORY latex/ DESTINATION share/doc/coriolis2/en/html/vlsisapd )
