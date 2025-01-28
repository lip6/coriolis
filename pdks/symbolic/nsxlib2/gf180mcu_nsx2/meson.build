
project(
  'pdk_gf180mcu_nsx2',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'gf180mcu_nsx2'

find_py = 'find libs.tech/coriolis/gf180mcu_nsx2 -type f -name "*.py"'
res = run_command('sh', '-c', find_py, check:true)
py_files = res.stdout().strip().split('\n')

py.install_sources( files(py_files), subdir: 'pdks/gf180mcu_nsx2' )

klayout_excluded = [
    'drc/README.md',
    'lvs/README.md',
    'pymacros/README.md',
    'pymacros/testing/drc_test/Makefile',
    'requirements.test.txt',
]

install_subdir( 'libs.tech/klayout' ,
  exclude_directories: ['pymacros/testing'],
  exclude_files: klayout_excluded,
  install_dir: pdks_dir )

