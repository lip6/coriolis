                   set ( LATEX_OUTPUT_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/latex )
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1,unicode"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

    add_latex_document ( dpgen.tex INPUTS man_dpgenadsb2f.tex
                                          man_dpgenand2.tex
                                          man_dpgenand3.tex
                                          man_dpgenand4.tex
                                          man_dpgenbuff.tex
                                          man_dpgenbuse.tex
                                          man_dpgenconst.tex
                                          man_dpgendff.tex
                                          man_dpgendfft.tex
                                          man_dpgenfifo.tex
                                          man_dpgeninv.tex
                                          man_dpgenmux2.tex
                                          man_dpgennand2mask.tex
                                          man_dpgennand2.tex
                                          man_dpgennand3.tex
                                          man_dpgennand4.tex
                                          man_dpgennbuse.tex
                                          man_dpgennmux2.tex
                                          man_dpgennor2mask.tex
                                          man_dpgennor2.tex
                                          man_dpgennor3.tex
                                          man_dpgennor4.tex
                                          man_dpgennul.tex
                                          man_dpgenor2.tex
                                          man_dpgenor3.tex
                                          man_dpgenor4.tex
                                          man_dpgenram.tex
                                          man_dpgenrf1d.tex
                                          man_dpgenrf1.tex
                                          man_dpgenrom2.tex
                                          man_dpgenrom4.tex
                                          man_dpgensff.tex
                                          man_dpgensfft.tex
                                          man_dpgenshift.tex
                                          man_dpgenshrot.tex
                                          man_dpgenxnor2mask.tex
                                          man_dpgenxnor2.tex
                                          man_dpgenxor2.tex
                                 )

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/dpgen )


        make_directory ( "${LATEX_OUTPUT_PATH}/dpgen" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${LATEX_OUTPUT_PATH}/dpgen/SoC.css" )

               install ( FILES ${LATEX_OUTPUT_PATH}/dpgen.tex
                               ${LATEX_OUTPUT_PATH}/dpgen.dvi
                               ${LATEX_OUTPUT_PATH}/dpgen.pdf DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${LATEX_OUTPUT_PATH}/dpgen DESTINATION ${htmlInstallDir} )
