librarymanager_includes = include_directories('.')

librarymanager_mocs = qt.compile_moc(
  headers: [
    'crlcore/ViewsModel.h',
    'crlcore/ViewsWidget.h',
    'crlcore/CellsModel.h',
    'crlcore/CellsWidget.h',
    'crlcore/LibrariesModel.h',
    'crlcore/LibrariesWidget.h',
    'crlcore/LibraryManager.h',
  ],
  include_directories: crlcore_includes,
  dependencies: Hurricane
)


librarymanager = shared_library(
  'librarymanager',

  'ViewsModel.cpp',
  'ViewsWidget.cpp',
  'CellDatas.cpp',
  'CellsModel.cpp',
  'CellsWidget.cpp',
  'LibrariesModel.cpp',
  'LibrariesWidget.cpp',
  'LibraryManager.cpp',
  librarymanager_mocs,

  link_with: crlcore,
  dependencies: [qt_deps, py_deps, boost, Hurricane, LefDef],
  include_directories: [crlcore_includes],
  install: true,
)
 

