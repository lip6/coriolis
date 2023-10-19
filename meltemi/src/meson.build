meltemi_py = files([
  'PyMeltemi.cpp',
  'PyMeltemiEngine.cpp',
  ])

meltemi = shared_library(
  'meltemi',
  'MeltemiEngine.cpp',
  meltemi_py,
  dependencies: [Hurricane, CrlCore, Coloquinte, Etesian, Flute, Anabatic, Katana],
  install: true,
)

py.extension_module(
  'Meltemi',
  meltemi_py,
  link_with: [configuration, meltemi],
  dependencies: [py_mod_deps, Hurricane, CrlCore, Coloquinte, Etesian, Flute, Anabatic, Katana],
  install: true,
  subdir: 'coriolis'
)

