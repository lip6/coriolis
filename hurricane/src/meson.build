hurricane_includes = include_directories(
  'hurricane',
  'utilities',
  'isobar',
  'configuration',
  'analog',
  'viewer'
)

hurricane_deps = [qt_deps, py_deps, rapidjson, boost]

subdir('hurricane')
subdir('utilities')
subdir('isobar')
subdir('configuration')
subdir('analog')
subdir('viewer')

py.extension_module(
  'Hurricane',
  isobar_sources,
  dependencies: [qt_deps, py_mod_deps, boost],
  link_with: [viewer, isobar],
  include_directories: hurricane_includes,
  install: true,
  subdir: 'coriolis'
)

Hurricane = declare_dependency(
  link_with: [viewer, analog, isobar, configuration, utilities, hurricane],
  include_directories: hurricane_includes,
  dependencies: [qt_deps, py_deps, boost, rapidjson]
)

