
project(
  'pdk_cmos_sx',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'cmos_sx'

py_files = [ 
  'libs.tech/coriolis/cmos_sx/__init__.py',
  'libs.tech/coriolis/cmos_sx/techno.py'
]

data_files = [ 
  'libs.tech/coriolis/cmos_sx/cmos.rds',
]

py.install_sources( files(py_files),    subdir: 'pdks/cmos_sx' )
py.install_sources( files(data_files),  subdir: 'pdks/cmos_sx' )
