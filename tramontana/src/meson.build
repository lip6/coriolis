tramontana_mocs = qt.compile_moc(
  headers: [
    'tramontana/GraphicTramontanaEngine.h',
    'tramontana/TabEquipotentials.h',
    'tramontana/EquipotentialWidget.h',
    'tramontana/EquipotentialsModel.h',
    'tramontana/EquipotentialsWidget.h',
    'tramontana/ShortCircuitWidget.h',
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
  'EquipotentialWidget.cpp',
  'EquipotentialsModel.cpp',
  'EquipotentialsWidget.cpp',
  'ShortCircuit.cpp',
  'ShortCircuitWidget.cpp',
  'GraphicTramontanaEngine.cpp',
  'QueryTiles.cpp',
  'SweepLine.cpp',
  'TabEquipotentials.cpp',
  'Tile.cpp',
  'Configuration.cpp',
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

