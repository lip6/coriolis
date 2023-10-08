
anabatic_py = files([
  'PyStyleFlags.cpp',
  'PyAnabatic.cpp'
  ])

anabatic = shared_library(
  'anabatic',
  'Constants.cpp',
  'Configuration.cpp',
  'Matrix.cpp',
  'EdgeCapacity.cpp',
  'Edge.cpp',
  'Edges.cpp',
  'GCell.cpp',
  'Dijkstra.cpp',
  'AutoContact.cpp',
  'AutoContactTerminal.cpp',
  'AutoContactTurn.cpp',
  'AutoContactHTee.cpp',
  'AutoContactVTee.cpp',
  'AutoSegment.cpp',
  'AutoSegments.cpp',
  'AutoHorizontal.cpp',
  'AutoVertical.cpp',
  'Session.cpp',
  'NetConstraints.cpp',
  'NetOptimals.cpp',
  'NetBuilder.cpp',
  'NetBuilderM2.cpp',
  'NetBuilderHV.cpp',
  'NetBuilderVH.cpp',
  'NetBuilderHybridVH.cpp',
  'ChipTools.cpp',
  'LayerAssign.cpp',
  'AntennaProtect.cpp',
  'PreRouteds.cpp',
  'AnabaticEngine.cpp',
  anabatic_py,

  dependencies: [Etesian],
  install: true,
)

py.extension_module(
  'Anabatic',
  anabatic_py,
  link_with: [anabatic],
  dependencies: [Etesian, py_mod_deps],
  install: true,
  subdir: 'coriolis'
)

