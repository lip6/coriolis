pyCRL_includes = include_directories('.')

pyCRL_files = files([
  'PyCRL.cpp',
  'PySystem.cpp',
  'PyBanner.cpp',
  'PyCatalog.cpp',
  'PyCatalogState.cpp',
  'PyEnvironment.cpp',
  'PyAllianceLibrary.cpp',
  'PyCellGauge.cpp',
  'PyRoutingGauge.cpp',
  'PyRoutingLayerGauge.cpp',
  'PyAllianceFramework.cpp',
  'PyToolBox.cpp',
  'PyToolEngine.cpp',
  'PyToolEngineCollection.cpp',
  'PyGraphicToolEngine.cpp',
  'PyAcmSigda.cpp',
  'PySpice.cpp',
  'PyBlif.cpp',
  'PyGds.cpp',
  'PyLefImport.cpp',
  'PyDefImport.cpp',
  'PyLefExport.cpp',
  'PyDefExport.cpp',
])

py.extension_module(
  'Constant',
  'PyConstant.cpp',
  dependencies: [qt_deps, py_mod_deps, boost, Hurricane],
  include_directories: crlcore_includes,
  subdir: 'coriolis',
  install: true
)

pycrlcore = shared_library(
  'pycrlcore',
  pyCRL_files,
  link_with: crlcore,
  dependencies: [qt_deps, boost, Hurricane],
  include_directories: crlcore_includes,
  install: true,
)


py.extension_module(
  'CRL',
  pyCRL_files,
  link_with: pycrlcore,
  dependencies: [qt_deps, py_mod_deps, boost, Hurricane],
  include_directories: crlcore_includes,
  subdir: 'coriolis',
  install: true
)
