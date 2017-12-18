# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<lefdef/src/def>" -*-

         include_directories ( ${LEFDEF_SOURCE_DIR}/src/def/def
                               ${LEFDEF_SOURCE_DIR}/src/def/include
                             )

                         set ( includes defiAlias.hpp 
                                        defiAssertion.hpp 
                                        defiBlockage.hpp 
                                        defiComponent.hpp 
                                        defiDebug.hpp 
                                        defiDefs.hpp 
                                        defiFPC.hpp 
                                        defiFill.hpp 
                                        defiGroup.hpp 
                                        defiIOTiming.hpp 
                                        defiKRDefs.hpp 
                                        defiMisc.hpp 
                                        defiNet.hpp 
                                        defiNonDefault.hpp 
                                        defiPartition.hpp 
                                        defiPath.hpp 
                                        defiPinCap.hpp 
                                        defiPinProp.hpp 
                                        defiProp.hpp 
                                        defiPropType.hpp 
                                        defiRegion.hpp 
                                        defiRowTrack.hpp 
                                        defiScanchain.hpp 
                                        defiSite.hpp 
                                        defiSlot.hpp 
                                        defiTimingDisable.hpp 
                                        defiUser.hpp 
                                        defiUtil.hpp 
                                        defiVia.hpp 
                                        defrCallBacks.hpp 
                                        defrData.hpp 
                                        defrReader.hpp 
                                        defrSettings.hpp 
                                        defwWriter.hpp 
                                        defwWriterCalls.hpp
                             )
      
                         set ( cpps     def.tab.cpp
                                        def_keywords.cpp
                                        defiAlias.cpp 
                                        defiAssertion.cpp 
                                        defiBlockage.cpp 
                                        defiComponent.cpp 
                                        defiDebug.cpp 
                                        defiFPC.cpp 
                                        defiFill.cpp 
                                        defiGroup.cpp 
                                        defiIOTiming.cpp 
                                        defiMisc.cpp 
                                        defiNet.cpp 
                                        defiNonDefault.cpp 
                                        defiPartition.cpp 
                                        defiPath.cpp 
                                        defiPinCap.cpp 
                                        defiPinProp.cpp 
                                        defiProp.cpp 
                                        defiPropType.cpp 
                                        defiRegion.cpp 
                                        defiRowTrack.cpp 
                                        defiScanchain.cpp 
                                        defiSite.cpp 
                                        defiSlot.cpp 
                                        defiTimingDisable.cpp 
                                        defiUtil.cpp 
                                        defiVia.cpp 
                                        defrCallbacks.cpp 
                                        defrData.cpp 
                                        defrReader.cpp 
                                        defrSettings.cpp 
                                        defwWriter.cpp 
                                        defwWriterCalls.cpp
                             )
      
                         set ( DefParserGrammar  ${LEFDEF_SOURCE_DIR}/src/def/def/def.y )
          add_custom_command ( MAIN_DEPENDENCY   ${DefParserGrammar}
                               COMMAND  ${BISON_EXECUTABLE} 
                               ARGS     -v -p defyy -d ${DefParserGrammar} -o def.tab.cpp 
                               COMMAND  mv
                               ARGS     def.tab.hpp ${LEFDEF_SOURCE_DIR}/src/def/def/def.tab.h
                               OUTPUT   def.tab.cpp
                             )
 set_source_files_properties ( def.tab.cpp GENERATED )

                 add_library ( def      ${cpps} )
       set_target_properties ( def      PROPERTIES VERSION 5.8 SOVERSION 5 )
      
                     install ( TARGETS  def DESTINATION lib${LIB_SUFFIX} )
                     install ( FILES    ${includes} DESTINATION include/lefdef ) 