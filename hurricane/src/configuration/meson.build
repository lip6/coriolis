
mocs = qt.compile_moc(
  headers: [
    'hurricane/configuration/ConfigurationDialog.h',
    'hurricane/configuration/ParameterWidget.h',
    'hurricane/configuration/ConfTabWidget.h',
    'hurricane/configuration/LogWidget.h',
    'hurricane/configuration/ConfigurationWidget.h',
    'hurricane/configuration/ConfEditorWidget.h'  ],
  include_directories: hurricane_includes
)

resources = qt.compile_resources(
  sources: 'Configuration.qrc'
)
configuration = shared_library(
  'configuration',
  'Parameter.cpp',
  'LayoutDescription.cpp',
  'Configuration.cpp',
  'ParameterWidget.cpp',
  'ConfTabWidget.cpp',
  'LogWidget.cpp',
  'ConfigurationWidget.cpp',
  'ConfigurationDialog.cpp',
  'ConfEditorWidget.cpp',
  mocs,
  resources,
  dependencies: [qt_deps, boost],
  link_with: [hurricane, isobar, utilities],
  include_directories: hurricane_includes
)

pytypemanager = shared_library(
  'pytypemanager',
  'ProxyProperty.cpp',
  'PyTypeManager.cpp',
  dependencies: [qt_deps, boost, py_deps],
  link_with: [hurricane, isobar],
  include_directories: hurricane_includes
)

py.extension_module(
  'Cfg',
  'PyParameter.cpp',
  'PyConfiguration.cpp',
  'PyLayoutDescription.cpp',
  'PyCfg.cpp',
  dependencies: [qt_deps, py_deps, boost],
  link_with: [isobar, hurricane, configuration, pytypemanager],
  include_directories: hurricane_includes
) 

py.extension_module(
  'Hurricane3',
  'PyDbU.cpp',
  'PyPoint.cpp',
  'PyBox.cpp',
  'PyDataBase.cpp',
  'PyTechnology.cpp',
  'PyLayerMask.cpp',
  'PyLayer.cpp',
  'PyMaterial.cpp',
  'PyBasicLayer.cpp',
  'PyViaLayer.cpp',
  'PyRegularLayer.cpp',
  'PyDiffusionLayer.cpp',
  'PyTransistorLayer.cpp',
  'PyHurricane3.cpp',
  dependencies: hurricane_deps,
  link_with: [hurricane, configuration, pytypemanager],
  include_directories: hurricane_includes
)
