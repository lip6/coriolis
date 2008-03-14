ADD_CUSTOM_TARGET(doc ALL cd ${HURRICANE_SOURCE_DIR}/doc/hurricane && ${DOXYGEN_EXECUTABLE} doxyfile)

INSTALL(DIRECTORY html/ DESTINATION /share/doc/en/html/hurricane)
