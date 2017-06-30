# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<PythonCpp>" -*-

               set ( htmlInstallDir  share/doc/coriolis2/en/html/PythonCpp )
               set ( latexInstallDir share/doc/coriolis2/en/latex/PythonCpp )

 add_custom_target ( doc_HTML ALL
                        cd ${DOCUMENTATION_SOURCE_DIR}/PythonCpp
                     && rst2html --link-stylesheet --stylesheet=../etc/SoC.css,../etc/SoC-ReST.css,../etc/Pygments.css PythonCpp.rst PythonCpp.html )
  add_dependencies ( doc_HTML ../etc/definitions.rst
                              ../etc/SoC.css
                              ../etc/SoC-ReST.css
                              ../etc/Pygments.css
                              PythonCpp.rst )

 add_custom_target ( doc_LaTeX  ALL
                        cd ${DOCUMENTATION_SOURCE_DIR}/PythonCpp
                     && export TEXINPUTS=../etc/images//:./images//:
                     && rst2latex --use-latex-toc --stylesheet=../etc/SoC-ReST.tex PythonCpp.rst  PythonCpp-raw.tex
                     && sed 's, \\& \\\\multicolumn{2}{l|}{,  \\& \\\\multicolumn{2}{p{0.6\\\\DUtablewidth}|}{,' PythonCpp-raw.tex > PythonCpp.tex
                     && pdflatex PythonCpp
                     && pdflatex PythonCpp
                   )
  add_dependencies ( doc_LaTeX ../etc/definitions.rst
                               ../etc/SoC-ReST.tex
                               PythonCpp.rst )

           install ( DIRECTORY images/
                               DESTINATION ${htmlInstallDir}/images
                               FILES_MATCHING PATTERN "*.png" )
           install ( FILES     PythonCpp.html      DESTINATION ${htmlInstallDir} )

           install ( DIRECTORY images/
                               DESTINATION ${latexInstallDir}/images
                               FILES_MATCHING PATTERN "*.pdf"
                                              PATTERN "*.eps"
                                              PATTERN "*.bb" )
 
           install ( FILES     PythonCpp.tex
                               PythonCpp.pdf       DESTINATION ${latexInstallDir} )
