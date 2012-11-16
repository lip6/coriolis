
               set ( htmlInstallDir  share/doc/coriolis2/en/html/crlcore )
               set ( latexInstallDir share/doc/coriolis2/en/latex/crlcore )

 add_custom_target ( documentation ALL cd ${CRLCORE_SOURCE_DIR}/doc/crlcore && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     socbook.cls          DESTINATION ${latexInstallDir} )
