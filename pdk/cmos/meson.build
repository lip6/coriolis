
project(
  'pdk_cmos',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'cmos'

cells_files = [ 
  'libs.tech/coriolis/cmos/__init__.py',
  'libs.tech/coriolis/cmos/techno.py'
]

py.install_sources( files(cells_files), subdir: 'pdks/cmos' )

install_subdir( 'libs.ref'  , install_dir: pdks_dir )
install_subdir( 'libs.tech' , install_dir: pdks_dir , exclude_directories: ['coriolis'] )
