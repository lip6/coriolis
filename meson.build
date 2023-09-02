project(
  'coriolis-eda',
  'c','cpp',
   version: '2.5.5',
   meson_version: '>= 1.2.0',
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
  '-Wno-return-type-c-linkage',
  '-Wno-unused-variable',
  '-Wno-#pragma-messages',
  '-Wno-unknown-pragmas',
  language: ['c','cpp']
)

py = import('python').find_installation(pure:false)
py_deps = dependency('python3-embed', required: true)
py_mod_deps = declare_dependency(dependencies: py_deps, compile_args: '-D__PYTHON_MODULE__=1')

qt = import('qt5')
qt_deps = dependency('qt5',
  modules: ['Core', 'Gui', 'Widgets', 'PrintSupport', 'Svg'],
  required:true
)

boost = dependency('boost', modules: ['program_options'], required: true)
libxml2 = dependency('libxml-2.0', required: true)
flex = find_program('flex', required: true)
bison = find_program('bison', required: true)

if build_machine.system() == 'darwin'
  add_project_link_arguments('-Wl,-rpath,@loader_path/', language: ['c','cpp'])
  #deeply hacky - to fix needs to sort issues
  # https://github.com/mesonbuild/meson-python/issues/3
  # https://github.com/mesonbuild/meson/issues/12113
  # and an issue with install_dir not being used by the rpath logic
  add_project_link_arguments('-Wl,-rpath,@loader_path/../lib/python' + py.language_version() + '/site-packages/coriolis', language: ['c','cpp'])
endif

subdir('hurricane')
subdir('lefdef')
subdir('crlcore')
subdir('flute')
