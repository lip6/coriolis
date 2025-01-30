
project(
  'pdk_sky130_vsc',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'sky130_vsc'

find_py = 'find libs.tech/coriolis -type f -name "*.py"'
res = run_command('sh', '-c', find_py, check:true)
py_files = res.stdout().strip().split('\n')

find_data = 'find libs.tech/coriolis -type f ! -name "*.py"'
res = run_command('sh', '-c', find_data, check:true)
data_files = res.stdout().strip().split('\n')

py.install_sources( files(py_files)       , subdir: 'pdks/sky130_vsc' )
py.install_sources( files(data_files)     , subdir: 'pdks/sky130_vsc' )

install_subdir( 'libs.tech/klayout' , install_dir: pdks_dir )

