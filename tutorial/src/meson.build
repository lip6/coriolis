tutorial_mocs = qt.compile_moc(
  headers: [
    'tutorial/GraphicTutorialEngine.h',
  ],
  dependencies: [Anabatic],
)

tutorial_py = files([
  'PyTutorial.cpp',
  'PyTutorialEngine.cpp',
  'PyGraphicTutorialEngine.cpp',
])


tutorial = shared_library(
  'tutorial',

  'GraphicTutorialEngine.cpp',
  'TutorialEngine.cpp',
  tutorial_mocs,
  tutorial_py,
  dependencies: [CrlCore],
  install: true,
)

py.extension_module(
  'Tutorial',

  tutorial_py,

  link_with: [tutorial],
  dependencies: [py_mod_deps, CrlCore],
  install: true,
  subdir: 'coriolis'
)

