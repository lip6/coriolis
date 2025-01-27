
project(
  'pdk_sg13g2_lsx',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'sg13g2_lsx'

py_files = [ 
  'libs.tech/coriolis/sg13g2_lsx/__init__.py',
  'libs.tech/coriolis/sg13g2_lsx/lsxlib.py',
  'libs.tech/coriolis/sg13g2_lsx/Sg13g2lsxSetup.py',
  'libs.tech/coriolis/sg13g2_lsx/techno_symb.py',
  'libs.tech/coriolis/sg13g2_lsx/techno.py',
]

data_files = [ 
  'libs.tech/coriolis/sg13g2_lsx/sg13g2_lsx.rds',
  'libs.tech/coriolis/sg13g2_lsx/symbolic.rds',
  'libs.tech/coriolis/sg13g2_lsx/symbolic.dreal',
  'libs.tech/coriolis/sg13g2_lsx/symbolic.graal',
  'libs.tech/coriolis/sg13g2_lsx/spimodel.cfg',
]

exp_files = [ 
  'libs.tech/coriolis/sg13g2_lsx/exp/lambda_lsx.exp',
  'libs.tech/coriolis/sg13g2_lsx/exp/sg13g2_lsx.main.exp',
  'libs.tech/coriolis/sg13g2_lsx/exp/sg13g2_lsx.rules.exp',
  'libs.tech/coriolis/sg13g2_lsx/exp/sg13g2_lsx.values.exp',
  'libs.tech/coriolis/sg13g2_lsx/exp/symbolic_lsx.rules.exp',
]

klayout_files = [ 
  'libs.tech/klayout/sg13g2.lydrc',
  'libs.tech/klayout/sg13g2.lyp',
  'libs.tech/klayout/sg13g2.lyt',
]

py.install_sources( files(py_files)       , subdir: 'pdks/sg13g2_lsx' )
py.install_sources( files(data_files)     , subdir: 'pdks/sg13g2_lsx' )
py.install_sources( files(exp_files)      , subdir: 'pdks/sg13g2_lsx' )
py.install_sources( files(klayout_files)  , subdir: 'pdks/sg13g2_lsx' )

install_subdir( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/cells' , install_dir: pdks_dir / 'sg13g2_lsx' )
