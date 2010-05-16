
               set ( htmlInstallDir  share/doc/en/html/viewer )
               set ( latexInstallDir share/doc/en/latex/viewer )

 add_custom_target ( documentation ALL cd ${HURRICANE_SOURCE_DIR}/doc/viewer && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
