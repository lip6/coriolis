project(
  'coriolis-eda',
  'c','cpp',
   version: '2.5.5',
   meson_version: '>= 0.63.0',
   default_options: [
     'buildtype=debugoptimized',
     'c_std=c99',
     'cpp_std=c++17',
   ],
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

add_project_arguments(
  '-Wno-deprecated-declarations',
  '-Wno-unused-function',
  '-Wno-unused-private-field',
  language: ['c','cpp']
)

py = import('python').find_installation()
py_deps = dependency('python3')
qt = import('qt5')
qt_deps = dependency('qt5', modules: ['Core', 'Gui', 'Widgets'])
boost = dependency('boost')

subdir('hurricane')
