# -*- mode: CMAKE; explicit-buffer-name: # "CMakeLists.txt<crlcore/doc/crlcore>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/crlcore )
               set ( latexInstallDir share/doc/coriolis2/en/latex/crlcore )
               set ( doxExtras       closed.png
                                     open.png
                                     tabs.css
                   )

 if (BUILD_DOC AND DOXYGEN_FOUND)
 add_custom_target ( documentation ALL
                        cd ${CRLCORE_SOURCE_DIR}/doc/crlcore
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )
 endif()

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     socbook.cls          DESTINATION ${latexInstallDir} )
