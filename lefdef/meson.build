lefdef_includes = include_directories('src/lef/lef', 'src/def/def')

subdir('src/lef')
subdir('src/def')

lef_generated =  custom_target('lef.tab.cpp',
  output: [ 'lef.tab.cpp', 'lef.tab.hpp' ],
  input: lef_grammer,
  command: [ bison, '-v', '-p lefyy', '-d', '-o@OUTPUT0@', 
            '@INPUT@' ]
)

def_generated =  custom_target('def.tab.cpp',
  output: [ 'def.tab.cpp', 'def.tab.hpp' ],
  input: def_grammer,
  command: [ bison, '-v', '-p defyy', '-d', '-o@OUTPUT0@', 
            '@INPUT@' ]
)

zlib = dependency('zlib')

lefdef = shared_library(
  'lefdef',
  lef_files,
  def_files,
  lef_generated,
  def_generated,

  dependencies: zlib,
  include_directories: lefdef_includes,

  cpp_args: [
    '-Wno-format-extra-args',
    '-Wno-error=unused-but-set-variable',
    '-Wno-reorder-ctor',
    '-Wno-format',
  ],
  install: true
)

LefDef = declare_dependency(
  link_with: lefdef,
  include_directories: lefdef_includes
)
