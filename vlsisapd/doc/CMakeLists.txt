ADD_CUSTOM_TARGET ( doc ALL cd ${VLSISAPD_SOURCE_DIR}/doc && ${DOXYGEN_EXECUTABLE} doxyfile )

INSTALL ( DIRECTORY html/  DESTINATION share/doc/coriolis2/en/html/vlsisapd )
INSTALL ( DIRECTORY latex/ DESTINATION share/doc/coriolis2/en/html/vlsisapd )
