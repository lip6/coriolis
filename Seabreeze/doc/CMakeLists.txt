# -*- mode: CMAKE; explicit-buffer-name: # "CMakeLists.txt<Seabreeze/doc>" -*-

        set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/Seabreeze )
        set ( latexInstallDir share/doc/coriolis2/en/latex/Seabreeze )
        set ( doxExtras       customHierarchy.html
                              closed.png
                              open.png
                              tabs.css
            )

	add_custom_target ( doc ALL
                        cd ${SEABREEZE_SOURCE_DIR}/doc
                     && ${DOXYGEN_EXECUTABLE} doxyfile
                     && cp -f ${doxExtras} html
                   )

        install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
        install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
        install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )

