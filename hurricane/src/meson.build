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

Hurricane = declare_dependency(
  link_with: [viewer,analog,configuration,isobar,utilities,hurricane],
  include_directories: hurricane_includes,
  dependencies: [qt_deps, py_deps, boost, rapidjson]
)

