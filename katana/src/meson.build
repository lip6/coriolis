katana_mocs = qt.compile_moc(
  headers: [
    'katana/GraphicKatanaEngine.h',
  ],
  dependencies: [Anabatic],
)

katana_py = files([
  'PyKatana.cpp',
  'PyKatanaEngine.cpp',
  'PyGraphicKatanaEngine.cpp',
  'PyKatanaFlags.cpp',
])


katana = shared_library(
  'katana',

  'Constants.cpp',
  'Configuration.cpp',
  'Block.cpp',
  'DataNegociate.cpp',
  'DataSymmetric.cpp',
  'TrackCost.cpp',
  'TrackElement.cpp',
  'TrackElements.cpp',
  'TrackSegment.cpp',
  'TrackSegmentRegular.cpp',
  'TrackSegmentNonPref.cpp',
  'TrackSegmentWide.cpp',
  'TrackFixedSegment.cpp',
  'TrackFixedSpan.cpp',
  'TrackMarker.cpp',
  'Track.cpp',
  'Tracks.cpp',
  'HorizontalTrack.cpp',
  'VerticalTrack.cpp',
  'RoutingPlane.cpp',
  'Session.cpp',
  'Manipulator.cpp',
  'SegmentFsm.cpp',
  'RoutingEvent.cpp',
  'RoutingEventQueue.cpp',
  'RoutingEventHistory.cpp',
  'RoutingEventLoop.cpp',
  'NegociateWindow.cpp',
  'PowerRails.cpp',
  'PreRouteds.cpp',
  'ProtectRoutingPads.cpp',
  'PreProcess.cpp',
  'BloatProfile.cpp',
  'GlobalRoute.cpp',
  'SymmetricRoute.cpp',
  'KatanaEngine.cpp',
  'GraphicKatanaEngine.cpp',

  katana_mocs,
  katana_py,
  dependencies: [Anabatic],
  install: true,
)

py.extension_module(
  'Katana',

  katana_py,

  link_with: [katana],
  dependencies: [py_mod_deps, Anabatic],
  install: true,
  subdir: 'coriolis'
)

