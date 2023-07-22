
                   set ( LATEX_OUTPUT_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/latex )
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1,unicode"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

    add_latex_document ( patterns.tex )

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/patterns )

        make_directory ( "${LATEX_OUTPUT_PATH}/patterns" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${LATEX_OUTPUT_PATH}/patterns/SoC.css" )

               install ( FILES ${LATEX_OUTPUT_PATH}/patterns.tex
                               ${LATEX_OUTPUT_PATH}/patterns.dvi
                               ${LATEX_OUTPUT_PATH}/patterns.pdf DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${LATEX_OUTPUT_PATH}/patterns DESTINATION ${htmlInstallDir} )
