# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<documentation>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(DOCUMENTATION)

 cmake_minimum_required(VERSION 2.4.0)

 option(BUILD_DOC "Build the documentation (html+pdf)" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 find_package(Sphinx    REQUIRED)
 setup_project_paths(CORIOLIS)

 set_cmake_policies()

#if(BUILD_DOC)
#  include(UseLATEX)
#endif(BUILD_DOC)
 
 add_subdirectory(examples)
 if(BUILD_DOC)
                 set ( htmlInstallDir  share/doc/coriolis2/en/html/main )
                 set ( pdfInstallDir   share/doc/coriolis2/en/pdf/main )

                 set ( pythonCppRst    PythonCpp/pdfHeader.rst
                                       PythonCpp/Introduction.rst
                                       PythonCpp/Configuration.rst
                                       PythonCpp/DBoStandalone.rst
                                       PythonCpp/DBoHierarchy.rst
                                       PythonCpp/NonDBo.rst
                                       PythonCpp/DbU.rst
                                       PythonCpp/Name.rst )
  
                 set ( usersGuideRst   UsersGuide/pdfHeader.rst
                                       UsersGuide/LicenseCredits.rst
                                       UsersGuide/Releases.rst
                                       UsersGuide/Installation.rst
                                       UsersGuide/Configuration.rst
                                       UsersGuide/ViewerTools.rst
                                       UsersGuide/ScriptsPlugins.rst )
  
                 set ( rdsRst          RDS/pdfHeader.rst
                                       RDS/RDSpage.rst )
  
   add_custom_target ( doc_HTML ALL
                          cd ${DOCUMENTATION_SOURCE_DIR}
                       && rm -rf _build
                       && sphinx-build -b html -d _build/doctrees . _build/html 
                       DEPENDS         etc/definitions.rst
                                       _static/SoC.css
                                       _static/www-SoC.css
                                       _static/SoC-ReST.css
                                       _static/pygments.css
                                       CrlCore/CrlCore.rst
                                       DpGen/DpGen.rst
                                       Hurricane/Hurricane.rst
                                       Patterns/Patterns.rst
                                       Stratus/Stratus.rst
                                       Unicorn/Unicorn.rst
                                       Viewer/Viewer.rst
                                       ${usersGuideRst}   UsersGuide/index.rst
                                       ${pythonCppRst}     PythonCpp/index.rst
                                       ${rdsRst}                 RDS/index.rst
                     )
  
   add_custom_target ( pdf_UsersGuide  ALL
                          cd ${DOCUMENTATION_SOURCE_DIR}/UsersGuide
                       && ../etc/doPdf.sh ${usersGuideRst} UsersGuide.rst 
                       DEPENDS         etc/definitions.rst
                                       etc/SoC-ReST.tex
                                       ${usersGuideRst} )
  
   add_custom_target ( pdf_PythonCpp  ALL
                          cd ${DOCUMENTATION_SOURCE_DIR}/PythonCpp
                       && ../etc/doPdf.sh ${pythonCppRst} PythonCpp.rst
                       DEPENDS         etc/definitions.rst
                                       etc/SoC-ReST.tex
                                       ${pythonCppRst} )
  
   add_custom_target ( pdf_RDS  ALL
                          cd ${DOCUMENTATION_SOURCE_DIR}/RDS
                       && ../etc/doPdf.sh ${rdsRst} RDS.rst
                       DEPENDS         etc/definitions.rst
                                       etc/SoC-ReST.tex
                                       ${pythonCppRst} )
  
             install ( DIRECTORY _build/html/               DESTINATION ${htmlInstallDir} )
             install ( FILES     RDS/RDS.pdf
                                 PythonCpp/PythonCpp.pdf
                                 UsersGuide/UsersGuide.pdf  DESTINATION ${pdfInstallDir} )
  
 endif(BUILD_DOC)

