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
  '-Wno-unused-but-set-variable',
  '-Wno-overloaded-virtual',
  language: ['c','cpp']
)

if get_option('check-database')
  add_project_arguments('-DCHECK_DATABASE')
endif

py = import('python').find_installation(pure:false)
py_deps = dependency('python3-embed', required: false)
if not py_deps.found()
  py_deps = dependency('python3', required: true)
endif

py_mod_deps = declare_dependency(dependencies: py_deps, compile_args: '-D__PYTHON_MODULE__=1')

qt = import('qt5')
qt_deps = dependency('qt5',
  modules: ['Core', 'Gui', 'Widgets', 'PrintSupport', 'Svg'],
  required:true
)

cmake =import('cmake')

boost = dependency('boost', modules: ['program_options'], required: true)
libxml2 = dependency('libxml-2.0', required: true)
flex = find_program('flex', required: true)
bison = find_program('bison', required: true)
thread_dep = dependency('threads')

if build_machine.system() == 'darwin'
  add_project_arguments('-mmacosx-version-min=13.0', language: ['c','cpp'])

  add_project_link_arguments('-Wl,-rpath,@loader_path/', language: ['c','cpp'])
  add_project_link_arguments('-Wl,-rpath,@loader_path/../lib/python' + py.language_version() + '/site-packages/coriolis', language: ['c','cpp'])

  qt5qwt6 = dependency('Qt5Qwt6', required: true)
  qwt_framework = qt5qwt6.get_pkgconfig_variable('libdir') / 'qwt.framework/Versions/6/Headers/'
  qwt = declare_dependency(include_directories: include_directories(qwt_framework), dependencies: qt5qwt6)

else
  qwt = dependency('qwt', required: false)
  if not qwt.found()
    qwt = dependency('Qt5Qwt6', required: true)
  endif
endif

#not ideal, hopefully coloquinte can switch to useing more modern targets, or to using meson!
coloquinte_sub = subproject('coloquinte')
Coloquinte = coloquinte_sub.get_variable('coloquinte_dep')

subdir('hurricane')
subdir('lefdef')
subdir('crlcore')
subdir('flute')
subdir('etesian')
subdir('anabatic')
subdir('katana')
subdir('equinox')
subdir('solstice')
subdir('oroshi')
subdir('bora')
subdir('unicorn')
subdir('cumulus')

