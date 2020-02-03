# -*- mode: CMAKE; explicit-buffer-name: # "CMakeLists.txt<oroshi/doc>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/oroshi )
               set ( latexInstallDir share/doc/coriolis2/en/latex/oroshi )
               set ( doxExtras       closed.png
                                     open.png
                                     tabs.css
                   )

 if(BUILD_DOC AND DOXYGEN_FOUND)
 add_custom_target ( doc ALL
                        cd ${OROSHI_SOURCE_DIR}/doc
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )
 endif()

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
