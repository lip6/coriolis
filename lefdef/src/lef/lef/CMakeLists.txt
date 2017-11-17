# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<lefdef/src/lef>" -*-

         include_directories ( ${LEFDEF_SOURCE_DIR}/src/lef/lef
                               ${LEFDEF_SOURCE_DIR}/src/lef/include
                             )

                         set ( includes lefiArray.hpp
                                        lefiCrossTalk.hpp
                                        lefiDebug.hpp
                                        lefiDefs.hpp
                                        lefiEncryptInt.hpp
                                        lefiKRDefs.hpp
                                        lefiLayer.hpp
                                        lefiMacro.hpp
                                        lefiMisc.hpp
                                        lefiNonDefault.hpp
                                        lefiProp.hpp
                                        lefiPropType.hpp
                                        lefiUnits.hpp
                                        lefiUser.hpp
                                        lefiUtil.hpp
                                        lefiVia.hpp
                                        lefiViaRule.hpp
                                        lefrCallBacks.hpp
                                        lefrData.hpp
                                        lefrReader.hpp
                                        lefrSettings.hpp
                                        lefwWriter.hpp
                                        lefwWriterCalls.hpp
                             )
      
                         set ( cpps     crypt.cpp
                                        lef.tab.cpp
                                        lef_keywords.cpp
                                        lefiArray.cpp
                                        lefiCrossTalk.cpp
                                        lefiDebug.cpp
                                        lefiEncryptInt.cpp
                                        lefiLayer.cpp
                                        lefiMacro.cpp
                                        lefiMisc.cpp
                                        lefiNonDefault.cpp
                                        lefiProp.cpp
                                        lefiPropType.cpp
                                        lefiTBExt.cpp
                                        lefiUnits.cpp
                                        lefiVia.cpp
                                        lefiViaRule.cpp
                                        lefrCallbacks.cpp
                                        lefrData.cpp
                                        lefrReader.cpp
                                        lefrSettings.cpp
                                        lefwWriter.cpp
                                        lefwWriterCalls.cpp
                             )
      
                         set ( LefParserGrammar  ${LEFDEF_SOURCE_DIR}/src/lef/lef/lef.y )
          add_custom_command ( SOURCE   ${LefParserGrammar}
                               COMMAND  ${BISON_EXECUTABLE} 
                               ARGS     -v -p lefyy -d ${LefParserGrammar} -o lef.tab.cpp 
                               COMMAND  mv
                               ARGS     lef.tab.hpp ${LEFDEF_SOURCE_DIR}/src/lef/lef/lef.tab.h
                               TARGET   LefParser
                               OUTPUTS  lef.tab.cpp
                             )
 set_source_files_properties ( lef.tab.cpp GENERATED )

                 add_library ( lef      ${cpps} )
       set_target_properties ( lef      PROPERTIES VERSION 5.8 SOVERSION 5 )
      
                     install ( TARGETS  lef DESTINATION lib${LIB_SUFFIX} )
                     install ( FILES    ${includes} DESTINATION include/lefdef ) 
