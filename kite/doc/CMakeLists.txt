# -*- explicit-buffer-name: "CMakeLists.txt<kite/doc>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/kite )
               set ( latexInstallDir share/doc/coriolis2/en/latex/kite )
               set ( doxExtras       customHierarchy.html
                                     closed.png
                                     open.png
                                     tabs.css
                   )

 add_custom_target ( doc ALL
                        cd ${KITE_SOURCE_DIR}/doc
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     ${doxExtras}         DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
