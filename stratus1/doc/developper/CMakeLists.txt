
                   set ( LATEX_OUTPUT_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/latex )
 if(BUILD_DOC)
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

    add_latex_document ( stratus_developper.tex INPUTS
                                                man_inst.tex
                                                man_model.tex
                                                man_net.tex
                                                man_stratus.tex
                                                see_also.tex
                       )
 endif()

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/stratus_developper )

        make_directory ( "${LATEX_OUTPUT_PATH}/stratus_developper" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${LATEX_OUTPUT_PATH}/stratus_developper/SoC.css" )

               install ( FILES ${LATEX_OUTPUT_PATH}/stratus_developper.tex
                               ${LATEX_OUTPUT_PATH}/stratus_developper.dvi
                               ${LATEX_OUTPUT_PATH}/stratus_developper.pdf DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${LATEX_OUTPUT_PATH}/stratus_developper DESTINATION ${htmlInstallDir} )
