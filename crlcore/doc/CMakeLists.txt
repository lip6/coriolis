
    add_latex_document ( README.tex )

                   set ( htmlInstallDir  share/doc/coriolis2/ )
                   set ( latexInstallDir share/doc/coriolis2/ )

 latex_get_output_path ( output_dir )
               install ( FILES ${output_dir}/README.tex
                               ${output_dir}/README.dvi
                               ${output_dir}/README.pdf
                               DESTINATION ${latexInstallDir} )
