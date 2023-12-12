project(
  'coriolis-eda',
  'c','cpp',
   version:  run_command('_build_utils/version.py').stdout().strip(),
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

build=true
if get_option('only-docs')
  build = false
endif

py = import('python').find_installation(pure:false)
py_orig_deps = dependency('python3', required: true)

#python_libpython_link = '-T' + meson.source_root() /  'libpython3.11.ld'
if build_machine.system() == 'linux'
  python_libpython_link = '-Wl,--allow-shlib-undefined'
elif build_machine.system() == 'darwin'
  python_libpython_link = '-undefined dynamic_lookup'
  # todo, investiage using -U 
endif

py_deps = declare_dependency(dependencies: py_orig_deps, link_args: python_libpython_link)
py_mod_deps = declare_dependency(dependencies: py_orig_deps, compile_args: '-D__PYTHON_MODULE__=1', link_args: python_libpython_link)

qt = import('qt5', disabler: true)
qt_deps = dependency('qt5',
  modules: ['Core', 'Gui', 'Widgets', 'PrintSupport', 'Svg'],
  required: build,
  disabler: true,
)

cmake =import('cmake')

boost = dependency('boost', modules: ['program_options'], required: build, disabler: true)
libxml2 = dependency('libxml-2.0', required: build, disabler: true)
flex = find_program('flex', required: build, disabler: true)
bison = find_program('bison', required: build, disabler: true)
thread_dep = dependency('threads', required: build, disabler: true)
sed = find_program('sed', required: build, disabler: true)

docdir = join_paths(get_option('datadir'), 'doc')
htmldir = join_paths(docdir,  'html')
docsiteurl = get_option('docs-siteurl')

if get_option('docs')
  doxygen = find_program('doxygen', required: true)
  dot = find_program('dot', required: true)
  pelican = find_program('pelican', required: true)
  rst2latex = find_program('rst2latex', 'rst2latex.py', required: true)
  pdflatex = find_program('pdflatex', required: true)
else
  doxygen = disabler()
  dot = disabler()
  pelican = disabler()
  rst2latex = disabler()
  pdflatex = disabler()
endif


if build_machine.system() == 'darwin'
  add_project_arguments('-mmacosx-version-min=13.0', language: ['c','cpp'])

  add_project_link_arguments('-Wl,-rpath,@loader_path/', language: ['c','cpp'])
  add_project_link_arguments('-Wl,-rpath,@loader_path/../lib/python' + py.language_version() + '/site-packages/coriolis', language: ['c','cpp'])

  qt5qwt6 = dependency('Qt5Qwt6', required: build, disabler: true)
  qwt_framework = qt5qwt6.get_pkgconfig_variable('libdir') / 'qwt.framework/Versions/6/Headers/'
  qwt = declare_dependency(include_directories: include_directories(qwt_framework), dependencies: qt5qwt6)

else
  qwt = dependency('qwt', required: false)
  if not qwt.found()
    qwt = dependency('Qt5Qwt6', required: build, disabler: true)
  endif
endif

#not ideal, hopefully coloquinte can switch to useing more modern targets, or to using meson!
if build
  coloquinte_sub = subproject('coloquinte')
  Coloquinte = coloquinte_sub.get_variable('coloquinte_dep')
else
  Coloquinte = disabler()
endif

subdir('hurricane')
subdir('lefdef')
subdir('crlcore')
subdir('flute')
subdir('etesian')
subdir('anabatic')
subdir('katana')
subdir('tramontana')
subdir('equinox')
subdir('solstice')
subdir('unicorn')
subdir('oroshi')
subdir('karakaze')
subdir('bora')
subdir('cumulus')
subdir('tutorial')
subdir('documentation')
