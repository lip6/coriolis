
                   set ( LATEX2HTML_CONVERTER_FLAGS "-html_version" "4.0,latin1,unicode"
                                                    "-style" "SoC.css"
                                                    "-init_file" "${CMAKE_CURRENT_SOURCE_DIR}/.latex2html-init"
                                                    CACHE STRING "Custom arguments passeds to latex2html" FORCE )

#   add_latex_document ( README.tex IMAGE_DIRS images )

                   set ( htmlInstallDir  share/doc/coriolis2/ )
                   set ( latexInstallDir share/doc/coriolis2/ )

#latex_get_output_path ( output_dir )

                 #file ( COPY "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" DESTINATION "${output_dir}/README" )
#       make_directory ( "${output_dir}/README" )
#      execute_process ( COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/SoC.css" "${output_dir}/README" )

#              install ( FILES ${output_dir}/README.tex
#                              ${output_dir}/README.dvi
#                              ${output_dir}/README.pdf
#                              DESTINATION ${latexInstallDir} )
#              install ( DIRECTORY ${output_dir}/README DESTINATION ${htmlInstallDir} )
               install ( FILES general-index.html DESTINATION ${htmlInstallDir} RENAME index.html )

IF(DOXYGEN_FOUND)
  add_subdirectory(crlcore)
ENDIF(DOXYGEN_FOUND)
add_subdirectory(UsersGuide)
