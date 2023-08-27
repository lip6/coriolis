crlcore_includes = include_directories(
  'properties',
  'acmsigda',
  'iccad04',
  'spice',
  'lefdef',
  'blif',
  'alliance/ap',
  'alliance/vst',
  'cif',
  'liberty',
  'toolbox',
  '../fonts'
) 
 
vst_parser_grammar =  custom_target('VstParserGrammar.cpp',
  output: [ 'VstParserGrammar.cpp', 'VstParserGrammar.hpp' ],
  input: 'alliance/vst/VstParserGrammar.yy',
  command: [ bison, '-v', '-d', '-p', 'VST', '-o', '@OUTPUT0@', '-y', '@INPUT@' ]
)

vst_parser_scanner =  custom_target('VstParserScanner.cpp',
  output: [ 'VstParserScanner.cpp' ],
  input: 'alliance/vst/VstParserScanner.ll',
  command: [ flex, '-PVST', '-o@OUTPUT@', '@INPUT@' ],
  depends: vst_parser_grammar
)

ioc_parser_grammar =  custom_target('IocParserGrammar.cpp',
  output: [ 'IocParserGrammar.cpp', 'IocParserGrammar.hpp' ],
  input: 'ioc/IocParserGrammar.yy',
  command: [ bison, '-v', '-d', '-p', 'Ioc', '-o', '@OUTPUT0@', '-y', '@INPUT@' ]
)
 
ioc_parser_scanner =  custom_target('IocParserScanner.cpp',
  output: [ 'IocParserScanner.cpp' ],
  input: 'ioc/IocParserScanner.ll',
  command: [ flex, '-PIoc', '-o@OUTPUT@', '@INPUT@' ],
  depends: ioc_parser_grammar
)



ccore_mocs = qt.compile_moc(
  headers: [
    'crlcore/GraphicToolEngine.h'
  ],
  include_directories: crlcore_includes,
  dependencies: Hurricane
)

crlcore = shared_library(
  'crlcore',
  'Utilities.cpp',
  'Memory.cpp',
  'Banner.cpp',
  'COptions.cpp',
  'Histogram.cpp',
  'OAParserDriver.cpp',
  'SearchPath.cpp',
  'Environment.cpp',
  'Catalog.cpp',
  'AllianceLibrary.cpp',
  'ParsersDrivers.cpp',
  'RoutingGauge.cpp',
  'CellGauge.cpp',
  'RoutingLayerGauge.cpp',
  'AllianceFramework.cpp',
  'ToolEngine.cpp',
  'GraphicToolEngine.cpp',
  
  'spice/SpiceBit.cpp',
  'spice/SpiceEntity.cpp',
  'spice/SpiceParser.cpp',
  'spice/SpiceDriver.cpp',
  'alliance/ap/ApParser.cpp',
  'alliance/ap/ApDriver.cpp',
  'gds/GdsDriver.cpp',
  'gds/GdsParser.cpp',

  'toolbox/HyperNetPortOccurrences.cpp',
  'toolbox/ToolBox.cpp',
  'toolbox/UniqueCellOccurrences.cpp',
  'toolbox/RoutingPads.cpp',
  'toolbox/NamingScheme.cpp',

  'alliance/vst/VhdlBit.cpp',
  'alliance/vst/VhdlSignal.cpp',
  'alliance/vst/VhdlPortMap.cpp',
  'alliance/vst/VhdlEntity.cpp',
  'alliance/vst/VstDriver.cpp',
  
  'properties/NetExtension.cpp',
  'properties/Measures.cpp',
  
  'lefdef/LefExport.cpp',
  'lefdef/DefExport.cpp',
  'lefdef/LefImport.cpp',
  'lefdef/DefImport.cpp',
  'lefdef/LefDefExtension.cpp',
  'iccad04/Iccad04Lefdef.cpp',
  'blif/BlifParser.cpp',
  
  'openaccess/OpenAccessParser.cpp',
  'openaccess/OpenAccessDriver.cpp',
  ccore_mocs,
  vst_parser_grammar,
  vst_parser_scanner,
  ioc_parser_grammar,
  ioc_parser_scanner,

  dependencies: [qt_deps, py_deps, libxml2, boost, Hurricane, LefDef],
  include_directories: [crlcore_includes, hurricane_includes]
)
  

