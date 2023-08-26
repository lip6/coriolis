py.extension_module(
  'Constant',
  'PyConstant.cpp',
  dependencies: [qt_deps, py_deps, boost, Hurricane],
  include_directories: crlcore_includes
  subdir: 'coriolis'
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

  dependencies: [qt_deps, py_deps, boost, Hurricane],
  include_directories: crlcore_includes
)

py.install_sources(
  '__init__.py',
  '',
  'helpers/__init__.py',
  'helpers/io.py',
  'helpers/utils.py',
  'helpers/overlay.py',
  'helpers/analogtechno.py',
  'helpers/technology.py',
  '',
  'technos/__init__.py',
  'technos/common',
  'technos/symbolic',
  'technos/node600',
  'technos/node180',
  'technos/node45',
  subdir: 'coriolis'
)

pypathdir = meson.current_build_dir()

# Create a test script that runs a python script that uses
# the C external module.  Must populate PYTHONPATH to include
# a path that includes the mymath.so dynamic library.
test('pyCRL',
    py,
    args : files('mymath_test.py'),
    env : ['PYTHONPATH=' + pypathdir])

