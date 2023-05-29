# -*- mode: CMAKE; explicit-buffer-name: # "CMakeLists.txt<katabatic/doc>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/katabatic )
               set ( latexInstallDir share/doc/coriolis2/en/latex/katabatic )
               set ( doxExtras       customHierarchy.html
                                     closed.png
                                     open.png
                                     tabs.css
                   )

 if(BUILD_DOC AND DOXYGEN_FOUND)
 add_custom_target ( doc ALL
                        cd ${KATABATIC_SOURCE_DIR}/doc
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
 endif()
