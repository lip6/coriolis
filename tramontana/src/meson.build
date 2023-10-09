tramontana_mocs = qt.compile_moc(
  headers: [
    'tramontana/GraphicTramontanaEngine.h',
    'tramontana/TabEquipotentials.h',
    'tramontana/EquipotentialsModel.h',
    'tramontana/EquipotentialsWidget.h',
  ],
  dependencies: [Anabatic],
)

tramontana_py = files([
  'PyTramontana.cpp',
  'PyTramontanaEngine.cpp',
  'PyGraphicTramontanaEngine.cpp',
])


tramontana = shared_library(
  'tramontana',

  'EquipotentialComponents.cpp',
  'Equipotential.cpp',
  'EquipotentialRelation.cpp',
  'EquipotentialsModel.cpp',
  'EquipotentialsWidget.cpp',
  'GraphicTramontanaEngine.cpp',
  'QueryTiles.cpp',
  'SweepLine.cpp',
  'TabEquipotentials.cpp',
  'Tile.cpp',
  'TramontanaEngine.cpp',
  tramontana_mocs,
  tramontana_py,
  dependencies: [Anabatic],
  install: true,
)

py.extension_module(
  'Tramontana',

  tramontana_py,

  link_with: [tramontana],
  dependencies: [py_mod_deps, Anabatic],
  install: true,
  subdir: 'coriolis'
)

