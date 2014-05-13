# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<hurricane/doc/hurricane>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/hurricane )
               set ( latexInstallDir share/doc/coriolis2/en/latex/hurricane )
               set ( doxExtras       customHierarchy.html
                                     closed.png
                                     open.png
                                     jquery.js
                                     dynsections.js
                   )

 add_custom_target ( doc ALL cd ${HURRICANE_SOURCE_DIR}/doc/hurricane && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     ${doxExtras}         DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
