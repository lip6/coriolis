etesian_mocs = qt.compile_moc(
  headers: [
    'etesian/GraphicEtesianEngine.h',
  ],
  dependencies: [Hurricane, CrlCore, Coloquinte],
)

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
  etesian_mocs,
  dependencies: [Hurricane, CrlCore, Coloquinte],
  install: true,
)

py.extension_module(
  'Etesian',
  'PyEtesian.cpp',
  'PyEtesianEngine.cpp',
  'PyGraphicEtesianEngine.cpp',

  link_with: [configuration],
  dependencies: [Hurricane, CrlCore, Coloquinte],
  install: true,
  subdir: 'coriolis'
)

