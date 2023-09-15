equinox_mocs = qt.compile_moc(
  headers: [
    'equinox/GraphicEquinoxEngine.h',
  ],
  dependencies: [CrlCore],
)


equinox_includes = include_directories('.', 'intervalTree/src')
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

  include_directories: equinox_includes,
  dependencies: [CrlCore],
  install: true,
)

