
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1,unicode"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

    add_latex_document ( patterns.tex )

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/patterns )

 latex_get_output_path ( output_dir )


                 #file ( COPY "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" DESTINATION "${output_dir}/README" )
        make_directory ( "${output_dir}/patterns" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${output_dir}/patterns" )

               install ( FILES ${output_dir}/patterns.tex
                               ${output_dir}/patterns.dvi
                               ${output_dir}/patterns.pdf
                               DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${output_dir}/patterns DESTINATION ${htmlInstallDir} )
