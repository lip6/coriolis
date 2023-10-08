unicorn_mocs = qt.compile_moc(
  headers: [
    'unicorn/UnicornGui.h',
    'unicorn/OpenCellDialog.h',
    'unicorn/SaveCellDialog.h',
    'unicorn/ImportCellDialog.h',
    'unicorn/ExportCellDialog.h',
  ],
  dependencies: [Katana, CrlCore],
)

unicorn_py = files([
  'PyUnicorn.cpp',
  'PyUnicornGui.cpp',
])


unicorn = shared_library(
  'unicorn',

  'ImportCell.cpp',
  'OpenCellDialog.cpp',
  'SaveCellDialog.cpp',
  'ImportCellDialog.cpp',
  'ExportCellDialog.cpp',
  'UnicornGui.cpp',

  unicorn_mocs,
  unicorn_py,
  dependencies: [Katana, CrlCore],
  install: true,
)

py.extension_module(
  'Unicorn',

  unicorn_py,

  link_with: [unicorn],
  dependencies: [py_mod_deps,Katana],
  install: true,
  subdir: 'coriolis'
)

