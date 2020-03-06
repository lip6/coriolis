# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<documentation>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(DOCUMENTATION)

 cmake_minimum_required(VERSION 2.4.0)

 set(ignoreVariables "${LIB_SUFFIX} ${CMAKE_INSTALL_DIR}")

 option(BUILD_DOC "Build the documentation (html+pdf)" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
#find_package(Sphinx)
 find_package(Pelican)
 setup_project_paths(CORIOLIS)

 set_cmake_policies()
 
 add_subdirectory(examples)

 set( htmlInstallDir  share/doc/coriolis2/en/html )
 if(BUILD_DOC)
   if(PELICAN_EXECUTABLE)
     message(STATUS "Pelican has been found, generate the documentation...")

     set ( pythonCppRst       content/pages/python-cpp/PythonCpp_HTML.rst
                              content/pages/python-cpp/Introduction.rst
                              content/pages/python-cpp/Configuration.rst
                              content/pages/python-cpp/DBoStandalone.rst
                              content/pages/python-cpp/DBoHierarchy.rst
                              content/pages/python-cpp/NonDBo.rst
                              content/pages/python-cpp/DbU.rst
                              content/pages/python-cpp/Name.rst )

     set ( pythonTutorialRst  content/pages/python-tutorial/PythonTutorial_HTML.rst
                              content/pages/python-tutorial/Introduction.rst
                              content/pages/python-tutorial/Environment.rst
                              content/pages/python-tutorial/CellNetComponent.rst
                              content/pages/python-tutorial/Collections.rst
                              content/pages/python-tutorial/CgtScript.rst
                              content/pages/python-tutorial/Netlist.rst
                              content/pages/python-tutorial/RealDesigns.rst
                              content/pages/python-tutorial/ToolEngines.rst
                              content/pages/python-tutorial/AdvancedTopics.rst )
 
     set ( usersGuideRst      content/pages/users-guide/UsersGuide_HTML.rst
                              content/pages/users-guide/LicenseCredits.rst
                              content/pages/users-guide/Releases.rst
                              content/pages/users-guide/DesignFlow.rst
                              content/pages/users-guide/Installation.rst
                              content/pages/users-guide/Configuration.rst
                              content/pages/users-guide/ViewerTools.rst
                              content/pages/users-guide/ScriptsPlugins.rst )

     set ( rdsRst             content/pages/rds/RDS_HTML.rst
                              content/pages/rds/RDSpage.rst )

     set ( stratusRst         content/pages/stratus/Stratus_HTML.rst
                              content/pages/stratus/Developper.rst
                              content/pages/stratus/Language.rst
                              content/pages/stratus/DpGen.rst
                              content/pages/stratus/Patterns.rst )

     add_custom_target ( doc_HTML ALL
                            cd ${DOCUMENTATION_SOURCE_DIR}
                         && rm -rf output
                         && ./build.py --pdfs --pelican
                         DEPENDS         etc/definitions.rst
                                         themes/nest-coriolis/static/css/nest.css
                                         themes/nest-coriolis/static/css/pygment.css
                                         themes/nest-coriolis/static/css/coriolis.css
                                         content/pages/documentation.rst
                                         content/pages/gitlab.rst
                                         content/pages/homepage.rst
                                         content/pages/symbolic-layout.rst
                                         ${usersGuideRst}
                                         ${pythonTutorialRst}
                                         ${pythonCppRst}
                                         ${rdsRst}
                                         ${stratusRst}
                       )

   else(PELICAN_EXECUTABLE)
     message(STATUS "Pelican has *not* been found, use the git supplied documentation.")
   endif(PELICAN_EXECUTABLE)
 endif(BUILD_DOC)
  
 install( DIRECTORY output/  DESTINATION ${htmlInstallDir} )


# Old part for Sphinx, keep it for now.
#set( htmlInstallDir  share/doc/coriolis2/en/html/main )
#set( pdfInstallDir   share/doc/coriolis2/en/pdf/main )

#if(BUILD_DOC)
#  if(SPHINX_EXECUTABLE)
#    message(STATUS "Sphinx has been found, generate the documentation...")
#                set ( pythonCppRst       PythonCpp/pdfHeader.rst
#                                         PythonCpp/Introduction.rst
#                                         PythonCpp/Configuration.rst
#                                         PythonCpp/DBoStandalone.rst
#                                         PythonCpp/DBoHierarchy.rst
#                                         PythonCpp/NonDBo.rst
#                                         PythonCpp/DbU.rst
#                                         PythonCpp/Name.rst )
#
#                set ( pythonTutorialRst  PythonTutorial/pdfHeader.rst
#                                         PythonTutorial/Introduction.rst
#                                         PythonTutorial/Environment.rst
#                                         PythonTutorial/CellNetComponent.rst
#                                         PythonTutorial/Collections.rst
#                                         PythonTutorial/CgtScript.rst
#                                         PythonTutorial/Netlist.rst
#                                         PythonTutorial/RealDesigns.rst
#                                         PythonTutorial/ToolEngines.rst
#                                         PythonTutorial/AdvancedTopics.rst )
# 
#                set ( usersGuideRst      UsersGuide/pdfHeader.rst
#                                         UsersGuide/LicenseCredits.rst
#                                         UsersGuide/Releases.rst
#                                         UsersGuide/DesignFlow.rst
#                                         UsersGuide/Installation.rst
#                                         UsersGuide/Configuration.rst
#                                         UsersGuide/ViewerTools.rst
#                                         UsersGuide/ScriptsPlugins.rst )
#
#                set ( confTechnoRst      ConfigurationTechnology/pdfHeader.rst
#                                         ConfigurationTechnology/Architecture.rst )
# 				          
#                set ( rdsRst             RDS/pdfHeader.rst
#                                         RDS/RDSpage.rst )
#
#                set (stratusRst          Stratus/pdfHeader.rst
#                                         Stratus/Developper.rst
#                                         Stratus/Language.rst
#                                         Stratus/DpGen.rst
#                                         Stratus/Patterns.rst )
# 
#  add_custom_target ( doc_HTML ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}
#                      && rm -rf _build
#                      && sphinx-build -b html -d _build/doctrees . _build/html 
#                      DEPENDS         etc/definitions.rst
#                                      _static/SoC.css
#                                      _static/www-SoC.css
#                                      _static/SoC-ReST.css
#                                      _static/pygments.css
#                                      CrlCore/CrlCore.rst
#                                      Hurricane/Hurricane.rst
#                                      Unicorn/Unicorn.rst
#                                      Viewer/Viewer.rst
#                                      ${usersGuideRst}      UsersGuide/index.rst
#                                      ${pythonTutorialRst}  PythonTutorial/index.rst
#                                      ${pythonCppRst}       PythonCpp/index.rst
#                                      ${rdsRst}             RDS/index.rst
#                                      ${stratusRst}         Stratus/index.rst
#                    )
# 
#  add_custom_target ( pdf_UsersGuide  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/UsersGuide
#                      && ../etc/doPdf.sh ${usersGuideRst} UsersGuide.rst 
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      ${usersGuideRst} )
# 
#  add_custom_target ( pdf_PythonTutorial  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/PythonTutorial
#                      && ../etc/doPdf.sh ${pythonTutorialRst} PythonTutorial.rst
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      PythonTutorial/definitions.rst
#                                      ${pythonTutorialRst} )
# 
#  add_custom_target ( pdf_Stratus  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/Stratus
#                      && ../etc/doPdf.sh ${stratusRst} Stratus.rst
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      ${stratusRst} )
# 
#  add_custom_target ( pdf_PythonCpp  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/PythonCpp
#                      && ../etc/doPdf.sh ${pythonCppRst} PythonCpp.rst
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      ${pythonCppRst} )
# 
#  add_custom_target ( pdf_ConfTechno  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/ConfigurationTechnology
#                      && ../etc/doPdf.sh ${confTechnoRst} ConfigurationTechnology.rst
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      ${confTechnoRst} )
# 
#  add_custom_target ( pdf_RDS  ALL
#                         cd ${DOCUMENTATION_SOURCE_DIR}/RDS
#                      && ../etc/doPdf.sh ${rdsRst} RDS.rst
#                      DEPENDS         etc/definitions.rst
#                                      etc/SoC-ReST.tex
#                                      ${rdsRst} )
#  else(SPHINX_EXECUTABLE)
#    message(STATUS "Sphinx has *not* been found, use the git supplied documentation.")
#  endif(SPHINX_EXECUTABLE)
#endif(BUILD_DOC)
# 
#install( DIRECTORY _build/html/               DESTINATION ${htmlInstallDir} )
#install( FILES     RDS/RDS.pdf
#                   PythonTutorial/PythonTutorial.pdf
#                   PythonCpp/PythonCpp.pdf
#                   UsersGuide/UsersGuide.pdf  DESTINATION ${pdfInstallDir} )
