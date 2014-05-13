# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<hurricane/doc/viewer>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/viewer )
               set ( latexInstallDir share/doc/coriolis2/en/latex/viewer )
               set ( doxExtras       closed.png
                                     open.png
                                     jquery.js
                                     dynsections.js
                   )

 add_custom_target ( documentation ALL cd ${HURRICANE_SOURCE_DIR}/doc/viewer && ${DOXYGEN_EXECUTABLE} doxyfile )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( FILES     ${docExtras}         DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
