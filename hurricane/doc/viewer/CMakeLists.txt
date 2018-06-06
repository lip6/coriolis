# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<hurricane/doc/viewer>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/viewer )
               set ( latexInstallDir share/doc/coriolis2/en/latex/viewer )
               set ( doxExtras       closed.png
                                     open.png
                                     tabs.css
                   )

 if(BUILD_DOC and DOXYGEN_FOUND)
 add_custom_target ( viewerDoc ALL
                        cd ${HURRICANE_SOURCE_DIR}/doc/viewer
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )
 endif()

           install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
           install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
           install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
