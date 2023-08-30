py.extension_module(
  'Constant',
  'PyConstant.cpp',
  dependencies: [qt_deps, py_deps, boost, Hurricane],
  include_directories: crlcore_includes,
  subdir: 'coriolis',
  install: true
) 

py.extension_module(
  'CRL',

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

  cpp_args: '-D__PYTHON_MODULE__=1',
  dependencies: [qt_deps, py_deps, boost, Hurricane],
  include_directories: crlcore_includes,
  install: true
)
pypathdir = meson.current_build_dir()

test('pyCRL',
    py,
    args: '-c import CRL',
    env: ['PYTHONPATH=' + pypathdir])
