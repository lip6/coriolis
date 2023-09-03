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
  'PyFlute.cpp',
  dependencies: [py_deps, Hurricane],
  include_directories: flute_includes,
  subdir: 'coriolis',
  install: true
) 


install_data(
  'POST9.dat',
  'POWV9.dat',
  install_dir: py.get_install_dir(pure: false) / 'coriolis'
)
