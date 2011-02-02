include_directories ( ${VLSISAPD_SOURCE_DIR}/src/liberty/src
                      ${PYTHON_INCLUDE_PATH}
                      ${Boost_INCLUDE_DIRS}
                    )
        
set ( includes vlsisapd/liberty/Attribute.h
               vlsisapd/liberty/Cell.h
               vlsisapd/liberty/FlipFlop.h
               vlsisapd/liberty/Library.h
               vlsisapd/liberty/Name.h
               vlsisapd/liberty/Pin.h
               vlsisapd/liberty/Timing.h
               vlsisapd/liberty/WireLoad.h
               vlsisapd/liberty/WireLoadArea.h
               vlsisapd/liberty/WireLoadSelection.h
    )
set ( cpps Attribute.cpp
           Cell.cpp
           FlipFlop.cpp
           Library.cpp
           Name.cpp
           Pin.cpp
           Timing.cpp
           WireLoad.cpp
           WireLoadArea.cpp
           WireLoadSelection.cpp
    )
set ( pycpps PyLiberty.cpp 
    )

set ( LibertyParserScanner    ${VLSISAPD_SOURCE_DIR}/src/liberty/src/LibertyParserScanner.ll  )
set ( LibertyParserGrammar    ${VLSISAPD_SOURCE_DIR}/src/liberty/src/LibertyParserGrammar.yy  )
set ( LibertyParserScannerCpp LibertyParserScanner.cpp )
set ( LibertyParserGrammarCpp LibertyParserGrammar.cpp )

add_custom_target ( LibertyParser echo "Creating Liberty parser" )
add_custom_command ( SOURCE   ${LibertyParserScanner}
                     COMMAND  ${FLEX_EXECUTABLE}
                     ARGS     -PLiberty_ -o${LibertyParserScannerCpp} ${LibertyParserScanner}
                     TARGET   LibertyParser
                     OUTPUTS  ${LibertyParserScannerCpp}
                   )
add_custom_command ( SOURCE   ${LibertyParserGrammar}
                     COMMAND  ${BISON_EXECUTABLE} 
                     ARGS     -d -v -p Liberty_ -y ${LibertyParserGrammar} -o ${LibertyParserGrammarCpp}
                     TARGET   LibertyParser
                     DEPENDS  ${LibertyParserScannerCpp}
                     OUTPUTS  ${LibertyParserGrammarCpp}
                   )
set ( liberty_parser_cpps ${LibertyParserScannerCpp}
                          ${LibertyParserGrammarCpp}
    )
set_source_files_properties ( ${LibertyParserScannerCpp} GENERATED )
set_source_files_properties ( ${LibertyParserGrammarCpp} GENERATED )

add_library ( liberty  ${cpps}
                       ${liberty_parser_cpps}
            )
set_target_properties ( liberty PROPERTIES VERSION 1.0 SOVERSION 1)
target_link_libraries ( liberty ${Boost_LIBRARIES}
                      )

install ( TARGETS  liberty DESTINATION lib${LIB_SUFFIX} )
install ( FILES    ${includes} DESTINATION include/vlsisapd/liberty ) 

if ( Boost_FOUND )
    add_library ( pyLIBERTY MODULE ${pycpps} )
    set_target_properties ( pyLIBERTY PROPERTIES
                                      OUTPUT_NAME "LIBERTY"
                                      PREFIX      ""
                          )
    target_link_libraries ( pyLIBERTY liberty ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} )
    install ( TARGETS pyLIBERTY DESTINATION ${PYTHON_SITE_PACKAGES} )
endif ( Boost_FOUND )
