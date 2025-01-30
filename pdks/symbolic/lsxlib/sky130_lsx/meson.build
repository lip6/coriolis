
project(
  'pdk_sky130_lsx',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'sky130_lsx'

find_py = 'find libs.tech/coriolis -type f -name "*.py"'
res = run_command('sh', '-c', find_py, check:true)
py_files = res.stdout().strip().split('\n')

data_files = [
  'libs.tech/coriolis/sky130_lsx/sky130_lsx.rds',
  'libs.tech/coriolis/sky130_lsx/symbolic.graal',
  'libs.tech/coriolis/sky130_lsx/symbolic.rds',
  'libs.tech/coriolis/sky130_lsx/lsxlib.lib',
  'libs.tech/coriolis/sky130_lsx/spimodel.cfg',
]

find_spi = 'find libs.tech/coriolis -type f -name "*.spi"'
res = run_command('sh', '-c', find_spi, check:true)
spi_files = res.stdout().strip().split('\n')

# *.exp and *.cir files are used for library generation and are not required in PDK packaging

py.install_sources( files(py_files)       , subdir: 'pdks/sky130_lsx' )
py.install_sources( files(data_files)     , subdir: 'pdks/sky130_lsx' )
py.install_sources( files(spi_files)      , subdir: 'pdks/sky130_lsx' )

install_subdir( 'libs.tech/klayout' , install_dir: pdks_dir )

