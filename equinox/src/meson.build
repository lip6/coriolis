equinox_mocs = qt.compile_moc(
  headers: [
    'equinox/GraphicEquinoxEngine.h',
  ],
  dependencies: [CrlCore],
)

equinox = shared_library(
  'equinox',

  'EquinoxFilters.cpp',
  'EquinoxCollections.cpp',
  'Equi.cpp',
  'TileSweepLine.cpp',
  'Tile.cpp',
  'Strategy.cpp',
  'EquinoxEngine.cpp',
  'UnionFind.cpp',
  'GraphicEquinoxEngine.cpp',
  'intervalTree/src/IntervalTree.cpp',
  equinox_mocs,

  include_directories: include_directories('intervalTree/src'),
  dependencies: [CrlCore],
  install: true,
)

