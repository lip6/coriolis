ADD_CUSTOM_TARGET(doc ALL cd ${CRLCORE_SOURCE_DIR}/doc/doxygen && ${DOXYGEN_EXECUTABLE} doxyfile)

INSTALL ( DIRECTORY html/ DESTINATION  share/doc/coriolis2/crlcore/doxygen )
