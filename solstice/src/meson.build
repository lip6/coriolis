solstice_mocs = qt.compile_moc(
  headers: [
    'solstice/RoutingErrorListModel.h',
    'solstice/RoutingErrorListWidget.h',
    'solstice/TabRoutingErrorList.h',
    'solstice/GraphicSolsticeEngine.h',
  ],
  dependencies: [Equinox],
)

solstice = shared_library(
  'solstice',

  'Brick.cpp',
  'RoutingError.cpp',
  'BrickSweepLine.cpp',
  'SolsticeEngine.cpp',
  'ShortCircuitError.cpp',
  'GraphicSolsticeEngine.cpp',
  'RoutingErrorListModel.cpp',
  'DisconnectError.cpp',
  'RoutingErrorListWidget.cpp',
  'TabRoutingErrorList.cpp',
  'RoutingErrorInformations.cpp',

  solstice_mocs,

  dependencies: [Equinox],
  install: true,
)

