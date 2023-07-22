# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<hurricane/doc/analog>" -*-

set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/analog )
set ( latexInstallDir share/doc/coriolis2/en/latex/analog )
set ( doxExtras       closed.png
	     open.png
	     tabs.css
)

add_custom_target ( analogDoc ALL
		cd ${HURRICANE_SOURCE_DIR}/doc/analog
	     && ${DOXYGEN_EXECUTABLE} doxyfile
	     && cp -f ${doxExtras} html
	   )

install ( DIRECTORY html/                DESTINATION ${htmlInstallDir} )
install ( DIRECTORY latex/               DESTINATION ${latexInstallDir} )
install ( FILES     asimbook.cls         DESTINATION ${latexInstallDir} )
