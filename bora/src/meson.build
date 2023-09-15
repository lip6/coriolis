bora_mocs = qt.compile_moc(
  headers: [
    'bora/SlicingDataModel.h',
    'bora/SlicingDataWidget.h',
    'bora/SlicingPlotWidget.h',
    'bora/SlicingWidget.h',
    'bora/GraphicBoraEngine.h',
  ],
  dependencies: [Katana, qwt],
)

bora_py = files([
  'PyParameterRange.cpp',
  'PyStepParameterRange.cpp',
  'PyMatrixParameterRange.cpp',
  'PySlicingNode.cpp',
  'PyDSlicingNode.cpp',
  'PyHSlicingNode.cpp',
  'PyRHSlicingNode.cpp',
  'PyRVSlicingNode.cpp',
  'PyVSlicingNode.cpp',
  'PyBoraEngine.cpp',
  'PyGraphicBoraEngine.cpp',
  'PyBora.cpp',
])


bora = shared_library(
  'bora',

  'BoxSet.cpp',
  'NodeSets.cpp',
  'ParameterRange.cpp',
  'HVSetState.cpp',
  'SlicingNode.cpp',
  'HVSlicingNode.cpp',
  'HSlicingNode.cpp',
  'VSlicingNode.cpp',
  'DSlicingNode.cpp',
  'RHVSlicingNode.cpp',
  'RHSlicingNode.cpp',
  'RVSlicingNode.cpp',
  'ChannelRouting.cpp',
  'BoraEngine.cpp',
  'Pareto.cpp',
  'SlicingDataModel.cpp',
  'SlicingDataWidget.cpp',
  'SlicingPlotWidget.cpp',
  'SlicingWidget.cpp',
  'GraphicBoraEngine.cpp',

  bora_mocs,
  bora_py,
  dependencies: [Katana, qwt],
  install: true,
)

py.extension_module(
  'Bora',

  bora_py,

  link_with: [bora],
  dependencies: [Katana, qwt],
  install: true,
  subdir: 'coriolis'
)

