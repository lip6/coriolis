flute_includes = include_directories('.')

flute = shared_library(
  'flute',

  'flute.cpp',
  'flute_mst.cpp',
  'dist.cpp',
  'dl.cpp',
  'err.cpp',
  'mst2.cpp',
  'heap.cpp',
  'neighbors.cpp',
  dependencies: [Hurricane],
  include_directories: flute_includes,
  install: true,
)


py.extension_module(
  'Flute',
  'PyFlute.cpp',
  dependencies: [py_mod_deps, Hurricane, CrlCore],
  include_directories: flute_includes,
  subdir: 'coriolis',
  install: true
)


py.install_sources(
  'POST9.dat',
  'POWV9.dat',
  subdir: 'coriolis',
)
