
project(
  'pdk_sg13g2_nsx2',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'sg13g2_nsx2'

py_files = [ 
  'libs.tech/coriolis/sg13g2_nsx2/__init__.py',
  'libs.tech/coriolis/sg13g2_nsx2/nsxlib2.py',
  'libs.tech/coriolis/sg13g2_nsx2/Sg13g2nsx2Setup.py',
  'libs.tech/coriolis/sg13g2_nsx2/techno_symb.py',
  'libs.tech/coriolis/sg13g2_nsx2/techno.py',
]

data_files = [
  'libs.tech/coriolis/sg13g2_nsx2/mos_tt.lib',
  'libs.tech/coriolis/sg13g2_nsx2/nsxlib2.lib',
  'libs.tech/coriolis/sg13g2_nsx2/spimodel.cfg',
  'libs.tech/coriolis/sg13g2_nsx2/symbolic.dreal',
  'libs.tech/coriolis/sg13g2_nsx2/symbolic.graal',
  'libs.tech/coriolis/sg13g2_nsx2/sg13g2_nsx2.rds',
]

klayout_files = [ 
  'libs.tech/klayout/sg13g2.lydrc',
  'libs.tech/klayout/sg13g2.lyp',
  'libs.tech/klayout/sg13g2.lyt',
]

py.install_sources( files(py_files)       , subdir: 'pdks/sg13g2_nsx2' )
py.install_sources( files(data_files)     , subdir: 'pdks/sg13g2_nsx2' )
py.install_sources( files(klayout_files)  , subdir: 'pdks/sg13g2_nsx2' )

install_subdir( 'libs.tech/coriolis/sg13g2_nsx2/spi' , install_dir: pdks_dir / 'sg13g2_lsx' )
