
               set ( htmlInstallDir  share/doc/coriolis2/en/html/unicorn )
               set ( latexInstallDir share/doc/coriolis2/en/latex/unicorn )

 add_custom_target ( documentation ALL cd ${UNICORN_SOURCE_DIR}/doc/unicorn && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     customSummary.html   DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     socbook.cls          DESTINATION ${latexInstallDir} )
