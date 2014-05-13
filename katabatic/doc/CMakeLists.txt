# -*- mode: CMAKE; explicit-buffer-name: # "CMakeLists.txt<katabatic/doc>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/katabatic )
               set ( latexInstallDir share/doc/coriolis2/en/latex/katabatic )
               set ( doxExtras       customHierarchy.html
                                     closed.png
                                     open.png
                                     jquery.js
                                     dynsections.js
                   )

 add_custom_target ( doc ALL cd ${KATABATIC_SOURCE_DIR}/doc && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     ${doxExtras}         DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
