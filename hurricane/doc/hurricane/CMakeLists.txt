
               set ( htmlInstallDir  /share/doc/en/html/hurricane )
               set ( latexInstallDir /share/doc/en/latex/hurricane )

 add_custom_target ( doc ALL cd ${HURRICANE_SOURCE_DIR}/doc/hurricane && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customHierarchy.html DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
