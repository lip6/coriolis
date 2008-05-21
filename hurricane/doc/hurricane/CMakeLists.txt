
               set ( htmlInstallDir /share/doc/en/html/hurricane )

 add_custom_target ( doc ALL cd ${HURRICANE_SOURCE_DIR}/doc/hurricane && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html                 DESTINATION ${htmlInstallDir}      )
           install ( FILES     customHierarchy.html DESTINATION ${htmlInstallDir}/html )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir}/html )
