
               set ( htmlInstallDir  share/doc/coriolis2/en/html/users-guide )
               set ( latexInstallDir share/doc/coriolis2/en/latex/users-guide )

 add_custom_target ( doc_LaTeX ALL cd ${CRLCORE_SOURCE_DIR}/doc/UsersGuide
                     && rst2html --link-stylesheet --stylesheet=./SoC.css,./Pygments.css UsersGuide_HTML.rst UsersGuide.html )

 add_custom_target ( doc_HTML  ALL cd ${CRLCORE_SOURCE_DIR}/doc/UsersGuide
                     && rst2latex --use-latex-toc --stylesheet=./socstyle.tex UsersGuide_LaTeX.rst UsersGuide.tex )

           install ( DIRECTORY images               DESTINATION ${htmlInstallDir} )
           install ( FILES     SoC.css
                               Pygments.css
                               UsersGuide.html      DESTINATION ${htmlInstallDir} )

           install ( FILES     socstyle.tex
                               UsersGuide.tex       DESTINATION ${latexInstallDir} )
