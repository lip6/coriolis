
               set ( htmlInstallDir  share/doc/coriolis2/en/html/viewer )
               set ( latexInstallDir share/doc/coriolis2/en/latex/viewer )

 add_custom_target ( documentation ALL cd ${HURRICANE_SOURCE_DIR}/doc/viewer && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
