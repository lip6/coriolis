# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/ccore>" -*-

# Try to find a reasonable CORIOLIS_TOP value.
 if( NOT("$ENV{CORIOLIS_USER_TOP}" STREQUAL "") )
   set ( CORIOLIS_USER_TOP "$ENV{CORIOLIS_USER_TOP}" )
 else( NOT("$ENV{CORIOLIS_USER_TOP}" STREQUAL "") )
   if( NOT("$ENV{CORIOLIS_TOP}" STREQUAL "") )
     set ( CORIOLIS_TOP "$ENV{CORIOLIS_TOP}" )
   endif( NOT("$ENV{CORIOLIS_TOP}" STREQUAL "") )
 endif( NOT("$ENV{CORIOLIS_USER_TOP}" STREQUAL "") )


#                  include ( ${QT_USE_FILE} )

       include_directories ( ${CRLCORE_SOURCE_DIR}/src/fonts
                             ${CRLCORE_SOURCE_DIR}/src/ccore
                             ${CRLCORE_SOURCE_DIR}/src/ccore/properties
                             ${CRLCORE_SOURCE_DIR}/src/ccore/bookshelf
                             ${CRLCORE_SOURCE_DIR}/src/ccore/acmsigda
                             ${CRLCORE_SOURCE_DIR}/src/ccore/ispd04
                             ${CRLCORE_SOURCE_DIR}/src/ccore/ispd05
                             ${CRLCORE_SOURCE_DIR}/src/ccore/iccad04
                             ${CRLCORE_SOURCE_DIR}/src/ccore/cspice
                             ${CRLCORE_SOURCE_DIR}/src/ccore/lefdef
                             ${CRLCORE_SOURCE_DIR}/src/ccore/blif
                             ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/ap
                             ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/vst
                             ${CRLCORE_SOURCE_DIR}/src/ccore/agds
                             ${CRLCORE_SOURCE_DIR}/src/ccore/cif
                             ${CRLCORE_SOURCE_DIR}/src/ccore/spice
                             ${CRLCORE_SOURCE_DIR}/src/ccore/liberty
                             ${CRLCORE_SOURCE_DIR}/src/ccore/toolbox
                             ${HURRICANE_INCLUDE_DIR}
                             ${CIF_INCLUDE_DIR}
                             ${CONFIGURATION_INCLUDE_DIR}
                             ${PYTHON_INCLUDE_PATH}
                             ${Boost_INCLUDE_DIR}
                             ${QtX_INCLUDE_DIR}
                           )

           add_definitions ( -DCORIOLIS_TOP="${CORIOLIS_TOP}"
                             -DSYS_CONF_DIR="${SYS_CONF_DIR}"
                             -DPYTHON_SITE_PACKAGES="${PYTHON_SITE_PACKAGES}"
                           )

                       set ( includes          crlcore/Utilities.h
                                               crlcore/Memory.h
                                               crlcore/Banner.h
                                               crlcore/Histogram.h
                                               crlcore/COptions.h
                                               crlcore/XmlParser.h
                                               crlcore/GdsDriver.h
                                               crlcore/OAParser.h
                                               crlcore/OADriver.h
                                               crlcore/CifDriver.h
                                               crlcore/SearchPath.h
                                               crlcore/Environment.h
                                               crlcore/Catalog.h
                                               crlcore/AllianceLibrary.h
                                               crlcore/ParsersDrivers.h
                                               crlcore/LefDefExtension.h
                                               crlcore/DefImport.h
                                               crlcore/DefExport.h
                                               crlcore/LefExport.h
                                               crlcore/Blif.h
                                               crlcore/AcmSigda.h
                                               crlcore/Iccad04Lefdef.h
                                               crlcore/Ispd04Bookshelf.h
                                               crlcore/Ispd05Bookshelf.h
                                               crlcore/Ioc.h
                                               crlcore/VhdlBit.h
                                               crlcore/VhdlSignal.h
                                               crlcore/VhdlEntity.h
                                               crlcore/VhdlPortMap.h
                                               crlcore/NetExtension.h
                                               crlcore/Measures.h
                                               crlcore/RoutingGauge.h
                                               crlcore/RoutingLayerGauge.h
                                               crlcore/CellGauge.h
                                               crlcore/SymbolicTechnologyParser.h
                                               crlcore/RealTechnologyParser.h
                                               crlcore/GraphicsParser.h
                                               crlcore/AllianceFramework.h
                                               crlcore/ToolEngine.h
                                               crlcore/ToolEngines.h
                                               crlcore/ToolBox.h
                                               crlcore/Hierarchy.h
                                               crlcore/CellPath.h
                                               crlcore/CellPaths.h
                                               crlcore/TimingEvent.h
                           )
                       set ( mocincludes       crlcore/GraphicToolEngine.h )
                       set ( ccore_cpps        Utilities.cpp
                                               Memory.cpp
                                               Banner.cpp
                                               COptions.cpp
                                               Histogram.cpp
                                               XmlParser.cpp
                                               GdsDriver.cpp
                                               OAParserDriver.cpp
                                               CifDriver.cpp
                                               SearchPath.cpp
                                               Environment.cpp
                                               Catalog.cpp
                                               AllianceLibrary.cpp
                                               ParsersDrivers.cpp
                                               RoutingGauge.cpp
                                               CellGauge.cpp
                                               RoutingLayerGauge.cpp
                                               SymbolicTechnologyParser.cpp
                                               RealTechnologyParser.cpp
                                               GraphicsParser.cpp
                                               AllianceFramework.cpp
                                               ToolEngine.cpp
                                               GraphicToolEngine.cpp
                           )
                       set ( spice_cpps        spice/SpiceParser.cpp
                                               spice/SpiceDriver.cpp
                                               spice/Spice.cpp
                           )
                       set ( bookshelf_cpps    bookshelf/BookshelfParser.cpp
                                               bookshelf/BookshelfDriver.cpp
                           )
                       set ( ap_cpps           alliance/ap/ApParser.cpp
                                               alliance/ap/ApDriver.cpp
                           )
                       set ( agds_cpps         agds/AgdsDriver.cpp
                           )
                       set ( cif_cpps          cif/CifDriver.cpp
                           )
                       set ( toolbox_cpps      toolbox/HyperNetPortOccurrences.cpp
                                               toolbox/ToolBox.cpp
                                               toolbox/UniqueCellOccurrences.cpp
                                               toolbox/RoutingPads.cpp
                                               toolbox/NamingScheme.cpp
                           )
                       set ( vst_driver_cpps   alliance/vst/VhdlBit.cpp
                                               alliance/vst/VhdlSignal.cpp
                                               alliance/vst/VhdlPortMap.cpp
                                               alliance/vst/VhdlEntity.cpp
                                               alliance/vst/VstDriver.cpp
                           )
                       set ( properties_cpps   properties/NetExtension.cpp
                                               properties/Measures.cpp
                           )
#                      set ( liberty_cpps      liberty/CellPath.cpp
#                                              liberty/LuTableTemplate.cpp
#                                              liberty/LibertyTechnology.cpp
#                                              liberty/TimingEvent.cpp
#                                              liberty/LuTable.cpp
#                          )
#                      set ( lefdef_cpps       lefdef/LefDef.h
#                                              lefdef/LefDefExtension.cpp
#                                              lefdef/LefParser.cpp
#                                              lefdef/DefParser.cpp
#                                              lefdef/DefDriver.cpp
#                          ) 
                       set ( lefdef_cpps       lefdef/LefExport.cpp
                                               lefdef/DefExport.cpp
                                               lefdef/DefImport.cpp
                                               lefdef/LefDefExtension.cpp
                           )
                       set ( iccad04_cpps      iccad04/Iccad04Lefdef.cpp )
                       set ( ispd04_cpps       ispd04/Ispd04Bookshelf.cpp )
                       set ( ispd05_cpps       ispd05/Ispd05Bookshelf.cpp )
                       set ( blif_cpps         blif/BlifParser.cpp )
 if ( LEFDEF_FOUND )
       include_directories ( ${LEFDEF_INCLUDE_DIR} )
 endif ( LEFDEF_FOUND )
 if ( OA_FOUND )
       include_directories ( ${OA_INCLUDE_DIR} )
 endif ( OA_FOUND )
                         set (openaccess_cpps openaccess/OpenAccessParser.cpp
                                              openaccess/OpenAccessDriver.cpp
                             )

                         set ( VstParserSourceDir  ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/vst )
                         set ( VstParserBinaryDir  ${CRLCORE_BINARY_DIR}/src/ccore/alliance/vst )
                         set ( VstParserScanner    ${VstParserSourceDir}/VstParserScanner.ll  )
                         set ( VstParserGrammar    ${VstParserSourceDir}/VstParserGrammar.yy  )
                         set ( VstParserScannerCpp ${VstParserBinaryDir}/VstParserScanner.cpp )
                         set ( VstParserGrammarCpp ${VstParserBinaryDir}/VstParserGrammar.cpp )
            add_subdirectory ( alliance )
           add_custom_target ( VstParser echo "Creating VST parser" )
          add_custom_command ( SOURCE   ${VstParserScanner}
                               COMMAND  ${FLEX_EXECUTABLE}
                               ARGS     -PVST -o${VstParserScannerCpp} ${VstParserScanner}
                               TARGET   VstParser
                               OUTPUTS  ${VstParserScannerCpp}
                             )
          add_custom_command ( SOURCE   ${VstParserGrammar}
                               COMMAND  ${BISON_EXECUTABLE} 
                               ARGS     -d -v -p VST -y ${VstParserGrammar} -o ${VstParserGrammarCpp}
                               TARGET   VstParser
                               DEPENDS  ${VstParserScannerCpp}
                               OUTPUTS  ${VstParserGrammarCpp}
                             )
         include_directories ( ${VstParserBinaryDir} )
                         set ( vst_parser_cpps ${VstParserScannerCpp}
                                               ${VstParserGrammarCpp}
                             )
 set_source_files_properties ( ${VstParserScannerCpp} GENERATED )
 set_source_files_properties ( ${VstParserGrammarCpp} GENERATED )


                         set ( IocParserSourceDir  ${CRLCORE_SOURCE_DIR}/src/ccore/ioc )
                         set ( IocParserBinaryDir  ${CRLCORE_BINARY_DIR}/src/ccore/ioc )
                         set ( IocParserScanner    ${IocParserSourceDir}/IocParserScanner.ll  )
                         set ( IocParserGrammar    ${IocParserSourceDir}/IocParserGrammar.yy  )
                         set ( IocParserScannerCpp ${IocParserBinaryDir}/IocParserScanner.cpp )
                         set ( IocParserGrammarCpp ${IocParserBinaryDir}/IocParserGrammar.cpp )
            add_subdirectory ( ioc )
           add_custom_target ( IocParser echo "Creating IOC parser" )
          add_custom_command ( SOURCE   ${IocParserScanner}
                               COMMAND  ${FLEX_EXECUTABLE}
                               ARGS     -PIoc -o${IocParserScannerCpp} ${IocParserScanner}
                               TARGET   IocParser
                               OUTPUTS  ${IocParserScannerCpp}
                             )
          add_custom_command ( SOURCE   ${IocParserGrammar}
                               COMMAND  ${BISON_EXECUTABLE} 
                               ARGS     -d -v -p Ioc -y ${IocParserGrammar} -o ${IocParserGrammarCpp}
                               TARGET   IocParser
                               DEPENDS  ${IocParserScannerCpp}
                               OUTPUTS  ${IocParserGrammarCpp}
                             )
         include_directories ( ${IocParserBinaryDir} )
                         set ( ioc_parser_cpps ${IocParserScannerCpp}
                                               ${IocParserGrammarCpp}
                             )
 set_source_files_properties ( ${IocParserScannerCpp} GENERATED )
 set_source_files_properties ( ${IocParserGrammarCpp} GENERATED )


# #                          set ( LibertyParserSourceDir  ${CRLCORE_SOURCE_DIR}/src/ccore/liberty )
# #                          set ( LibertyParserBinaryDir  ${CRLCORE_BINARY_DIR}/src/ccore/liberty )
# #                          set ( LibertyParserScanner    ${LibertyParserSourceDir}/LibertyParserScanner.ll  )
# #                          set ( LibertyParserGrammar    ${LibertyParserSourceDir}/LibertyParserGrammar.yy  )
# #                          set ( LibertyParserScannerCpp ${LibertyParserBinaryDir}/LibertyParserScanner.cpp )
# #                          set ( LibertyParserGrammarCpp ${LibertyParserBinaryDir}/LibertyParserGrammar.cpp )
# #             add_subdirectory ( liberty )
# #            add_custom_target ( LibertyParser echo "Creating LIBERTY parser" )
# #           add_custom_command ( SOURCE   ${LibertyParserScanner}
#                                COMMAND  ${FLEX_EXECUTABLE}
#                                ARGS     -PLIBERTY -o${LibertyParserScannerCpp} ${LibertyParserScanner}
#                                TARGET   LibertyParser
#                                OUTPUTS  ${LibertyParserScannerCpp}
#                              )
#           add_custom_command ( SOURCE   ${LibertyParserGrammar}
#                                COMMAND  ${BISON_EXECUTABLE} 
#                                ARGS     -d -v -p LIBERTY -y ${LibertyParserGrammar} -o ${LibertyParserGrammarCpp}
#                                TARGET   LibertyParser
#                                DEPENDS  ${LibertyParserScannerCpp}
#                                OUTPUTS  ${LibertyParserGrammarCpp}
#                              )
#          include_directories ( ${LibertyParserBinaryDir} )
#                          set ( liberty_parser_cpps ${LibertyParserScannerCpp}
#                                                    ${LibertyParserGrammarCpp}
#                              )
#  set_source_files_properties ( ${LibertyParserScannerCpp} GENERATED )
#  set_source_files_properties ( ${LibertyParserGrammarCpp} GENERATED )

                         set ( AcmSigdaParserSourceDir  ${CRLCORE_SOURCE_DIR}/src/ccore/acmsigda )
                         set ( AcmSigdaParserBinaryDir  ${CRLCORE_BINARY_DIR}/src/ccore/acmsigda )
                         set ( AcmSigdaParserScanner    ${AcmSigdaParserSourceDir}/AcmSigdaParserScanner.ll  )
                         set ( AcmSigdaParserGrammar    ${AcmSigdaParserSourceDir}/AcmSigdaParserGrammar.yy  )
                         set ( AcmSigdaParserScannerCpp ${AcmSigdaParserBinaryDir}/AcmSigdaParserScanner.cpp )
                         set ( AcmSigdaParserGrammarCpp ${AcmSigdaParserBinaryDir}/AcmSigdaParserGrammar.cpp )
            add_subdirectory ( acmsigda )
           add_custom_target ( AcmSigdaParser echo "Creating ACM/SIGDA parser" )
          add_custom_command ( SOURCE   ${AcmSigdaParserScanner}
                               COMMAND  ${FLEX_EXECUTABLE}
                               ARGS     -PAcmSigda_ -o${AcmSigdaParserScannerCpp} ${AcmSigdaParserScanner}
                               TARGET   AcmSigdaParser
                               OUTPUTS  ${AcmSigdaParserScannerCpp}
                             )
          add_custom_command ( SOURCE   ${AcmSigdaParserGrammar}
                               COMMAND  ${BISON_EXECUTABLE} 
                               ARGS     -d -v -p AcmSigda_ -y ${AcmSigdaParserGrammar} -o ${AcmSigdaParserGrammarCpp}
                               TARGET   AcmSigdaParser
                               DEPENDS  ${AcmSigdaParserScannerCpp}
                                        crlcore/AcmSigda.h
                               OUTPUTS  ${AcmSigdaParserGrammarCpp}
                             )
         include_directories ( ${AcmSigdaParserBinaryDir} )
                         set ( acmsigda_parser_cpps ${AcmSigdaParserScannerCpp}
                                                    ${AcmSigdaParserGrammarCpp}
                             )
 set_source_files_properties ( ${AcmSigdaParserScannerCpp} GENERATED )
 set_source_files_properties ( ${AcmSigdaParserGrammarCpp} GENERATED )


                qtX_wrap_cpp ( moc_cpps ${mocincludes} )

                 add_library ( crlcore  ${ccore_cpps}
                                        ${moc_cpps}
                                        ${ap_cpps}
                                        ${agds_cpps}
                                        ${cif_cpps}
                                        ${toolbox_cpps}
                                        ${vst_parser_cpps}
                                        ${vst_driver_cpps}
                                        ${properties_cpps}
                                        ${ioc_parser_cpps}
                                        ${liberty_cpps}
                                        ${liberty_parser_cpps}
                                        ${bookshelf_cpps}
                                        ${acmsigda_parser_cpps}
                                        ${iccad04_cpps}
                                        ${ispd04_cpps}
                                        ${ispd05_cpps}
                                        ${blif_cpps}
                                        ${spice_cpps}
                                        ${lefdef_cpps}
                                        ${openaccess_cpps}
                             )
       set_target_properties ( crlcore  PROPERTIES VERSION 1.0 SOVERSION 1 )
       target_link_libraries ( crlcore  ${HURRICANE_PYTHON_LIBRARIES}
                                        ${HURRICANE_GRAPHICAL_LIBRARIES}
                                        ${HURRICANE_LIBRARIES}
                                        ${BOOKSHELF_LIBRARY}
                                        ${CONFIGURATION_LIBRARY}
                                        ${CIF_LIBRARY}
                                        ${AGDS_LIBRARY}
                                        ${UTILITIES_LIBRARY}
                                        ${LEFDEF_LIBRARIES}
                                        ${OA_LIBRARIES}
                                        ${QtX_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        ${LIBXML2_LIBRARIES}
                                        ${PYTHON_LIBRARIES} -lutil
                             )

                     install ( TARGETS  crlcore DESTINATION lib${LIB_SUFFIX} )
                     install ( FILES    ${includes} ${mocincludes} DESTINATION include/coriolis2/crlcore ) 
