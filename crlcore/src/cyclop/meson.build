cyclop_mocs = qt.compile_moc(
  headers: [
    'Cyclop.h',
    'OpenCellDialog.h',
  ],
  include_directories: crlcore_includes,
  dependencies: Hurricane
)

executable(
  'cyclop',

  'OpenCellDialog.cpp',
  'DemoGo.cpp',
  'Cyclop.cpp',
  'CyclopMain.cpp',
  cyclop_mocs,

  include_directories: crlcore_includes,
  link_with: crlcore,
  dependencies: Hurricane,
  install: true
  )
