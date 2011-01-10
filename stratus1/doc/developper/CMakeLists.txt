
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

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/stratus_developper )

 latex_get_output_path ( output_dir )


                 #file ( COPY "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" DESTINATION "${output_dir}/README" )
        make_directory ( "${output_dir}/stratus_developper" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${output_dir}/stratus_developper" )

               install ( FILES ${output_dir}/stratus_developper.tex
                               ${output_dir}/stratus_developper.dvi
                               ${output_dir}/stratus_developper.pdf
                               DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${output_dir}/stratus_developper DESTINATION ${htmlInstallDir} )
