etesian_mocs = qt.compile_moc(
  headers: [
    'etesian/GraphicEtesianEngine.h',
  ],
  dependencies: [Hurricane, CrlCore, Coloquinte],
)

etesian_py = files([
  'PyEtesian.cpp',
  'PyEtesianEngine.cpp',
  'PyGraphicEtesianEngine.cpp',
  ])

etesian = shared_library(
  'etesian',
  'Configuration.cpp',
  'HFNS.cpp',
  'AddFeeds.cpp',
  'Placement.cpp',
  'FlattenPower.cpp',
  'FeedCells.cpp',
  'BufferCells.cpp',
  'BloatCells.cpp',
  'BloatProperty.cpp',
  'EtesianEngine.cpp',
  'GraphicEtesianEngine.cpp',
  etesian_py,
  etesian_mocs,
  dependencies: [Hurricane, CrlCore, Coloquinte],
  install: true,
)

py.extension_module(
  'Etesian',
  etesian_py,
  link_with: [configuration, etesian],
  dependencies: [py_mod_deps, Hurricane, CrlCore, Coloquinte],
  install: true,
  subdir: 'coriolis'
)

