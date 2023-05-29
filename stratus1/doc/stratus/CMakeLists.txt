
                   set ( LATEX_OUTPUT_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/latex )
 if(BUILD_DOC)
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1,unicode"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

    add_latex_document ( stratus.tex INPUTS man_alim_connectors.tex
                                            man_alim_rail.tex
                                            man_arithmetic.tex
                                            man_boolean.tex
                                            man_buff.tex
                                            man_comp.tex
                                            man_const.tex
                                            man_copy_up_segment.tex
                                            man_def_ab.tex
                                            man_example.tex
                                            man_fill_cell.tex
                                            man_generate.tex
                                            man_get_ref.tex
                                            man_inst.tex
                                            man_library.tex
                                            man_mux.tex
                                            man_net.tex
                                            man_new.tex
                                            man_pads.tex
                                            man_place_bottom.tex
                                            man_place_centric.tex
                                            man_place_contact.tex
                                            man_place_glu.tex
                                            man_place_left.tex
                                            man_place_pin.tex
                                            man_place_ref.tex
                                            man_place_right.tex
                                            man_place_segment.tex
                                            man_place.tex
                                            man_place_top.tex
                                            man_power_ring.tex
                                            man_reg.tex
                                            man_resize_ab.tex
                                            man_route_ck.tex
                                            man_set_ref_ins.tex
                                            man_shift.tex
                                            man_stimuli.tex
                                            man_stratus.tex
                                            see_also.tex
                                 IMAGE_DIRS images )

                   set ( htmlInstallDir  share/doc/coriolis2/en/html/doc/ )
                   set ( latexInstallDir share/doc/coriolis2/en/latex/stratus/ )

        make_directory ( "${LATEX_OUTPUT_PATH}/stratus" )
       execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${LATEX_OUTPUT_PATH}/stratus/SoC.css" )

               install ( FILES ${LATEX_OUTPUT_PATH}/stratus.tex
                               ${LATEX_OUTPUT_PATH}/stratus.dvi
                               ${LATEX_OUTPUT_PATH}/stratus.pdf DESTINATION ${latexInstallDir} )
               install ( DIRECTORY ${LATEX_OUTPUT_PATH}/stratus DESTINATION ${htmlInstallDir} )
 endif()
