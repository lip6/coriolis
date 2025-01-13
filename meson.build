project(
  'coriolis-eda',
  'c','cpp',
   version:  run_command('_build_utils/version.py').stdout().strip(),
   meson_version: '>= 1.2.0',
   default_options: [
     'buildtype=debugoptimized',
     'c_std=c99',
     'cpp_std=c++17',
     'b_lundef=false',
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

# Using libbfd for proper backtrace in Python bindings
if build_machine.system() == 'darwin'
  binutils_prefix = '/opt/homebrew/opt/binutils'
else
  binutils_prefix = '/usr'
endif

libbfd    = cpp.find_library('bfd',           dirs: [binutils_prefix / 'lib'], required: true)
if libbfd.found()
    add_project_arguments('-DHAVE_LIBBFD', language: ['c', 'cpp'])

    libiberty   = cpp.find_library('iberty',  dirs: [binutils_prefix / 'lib'], required: true)
    zlib        = cpp.find_library('z',       required: true)
    libsframe   = cpp.find_library('sframe',  dirs: [binutils_prefix / 'lib'], required: true)
    
    libbfd_deps = declare_dependency(
        dependencies: [libbfd, libiberty, zlib, libsframe],
        include_directories: include_directories(binutils_prefix / 'include'),
        link_args: ['-Wl,-rpath,' + binutils_prefix / 'lib'],
    )
else
    libbfd_deps = disabler()
endif

if get_option('check-database')
  add_project_arguments('-DCHECK_DATABASE')
endif

py = import('python').find_installation(pure:false)
py_deps = dependency('python3', required: true)

py_mod_deps = declare_dependency(dependencies: [py_deps, libbfd_deps], compile_args: '-D__PYTHON_MODULE__=1')
qt = import('qt5', disabler: true)

sed = find_program('sed',  disabler: true)
docdir = join_paths(get_option('datadir'), 'doc', 'coriolis')
htmldir = join_paths(docdir,  'html')
docsiteurl = get_option('docs-siteurl')

if not get_option('only-docs')
  qt_deps = dependency('qt5',
    modules: ['Core', 'Gui', 'Widgets', 'PrintSupport', 'Svg'],
    disabler: true,
  )

  boost = dependency('boost', modules: ['program_options'],  disabler: true)
  libxml2 = dependency('libxml-2.0',  disabler: true)
  flex = find_program('flex',  disabler: true)
  bison = find_program('bison',  disabler: true)
  thread_dep = dependency('threads',  disabler: true)
  if build_machine.system() == 'darwin'
    add_project_arguments('-mmacosx-version-min=13.0', language: ['c','cpp'])

    add_project_link_arguments('-Wl,-rpath,@loader_path/', language: ['c','cpp'])
    add_project_link_arguments('-Wl,-rpath,@loader_path/../lib/python' + py.language_version() + '/site-packages/coriolis', language: ['c','cpp'])

    qt5qwt6 = dependency('Qt5Qwt6',  disabler: true)
    qwt_framework = qt5qwt6.get_pkgconfig_variable('libdir') / 'qwt.framework/Versions/6/Headers/'
    qwt = declare_dependency(include_directories: include_directories(qwt_framework), dependencies: qt5qwt6)

  else
    qwt = dependency('qwt', required: false)
    if not qwt.found()
      qwt = dependency('Qt5Qwt6',  disabler: true)
    endif
  endif

  coloquinte_sub = subproject('coloquinte')
  Coloquinte = coloquinte_sub.get_variable('coloquinte_dep')
else
  qt_deps = disabler()
  boost = disabler()
  libxml2 = disabler()
  flex = disabler()
  bison = disabler()
  thread_dep = disabler()
  qwt = disabler()
  Coloquinte = disabler()

endif


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


subdir('hurricane')
subdir('lefdef')
subdir('crlcore')
subdir('flute')
subdir('etesian')
subdir('anabatic')
subdir('katana')
subdir('tramontana')
subdir('oroshi')
subdir('karakaze')
subdir('bora')
subdir('unicorn')
subdir('cumulus')
subdir('tutorial')
subdir('documentation')
