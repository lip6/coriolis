
mocs = qt.compile_moc(
  headers: [
    'hurricane/configuration/ConfigurationDialog.h',
    'hurricane/configuration/ParameterWidget.h',
    'hurricane/configuration/ConfTabWidget.h',
    'hurricane/configuration/LogWidget.h',
    'hurricane/configuration/ConfigurationWidget.h',
    'hurricane/configuration/ConfEditorWidget.h'  ],
  include_directories: includes
)

resources = qt.compile_resources(
  sources: 'Configuration.qrc'
)
configuration = library(
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
  link_with: [hurricane, isobar],
  include_directories: includes
)

pytypemanager = library(
  'pytypemanager',
  'ProxyProperty.cpp',
  'PyTypeManager.cpp',
  dependencies: [qt_deps, boost, py_deps],
  link_with: [hurricane, isobar],
  include_directories: includes
)

