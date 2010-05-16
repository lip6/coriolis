
               set ( htmlInstallDir  share/doc/en/html/katabatic )
               set ( latexInstallDir share/doc/en/latex/katabatic )

 add_custom_target ( doc ALL cd ${KATABATIC_SOURCE_DIR}/doc && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customHierarchy.html DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
