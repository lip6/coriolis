subdir('src/ccore')
subdir('src/LibraryManager')
subdir('src/cyclop')
subdir('src/pyCRL')
subdir('python')
subdir('doc/crlcore')

CrlCore = declare_dependency(
  link_with: [crlcore, librarymanager, pycrlcore],
  include_directories: [crlcore_includes, pyCRL_includes, librarymanager_includes],
  dependencies: [qt_deps, py_deps, boost, rapidjson, Hurricane]
)

