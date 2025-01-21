
project(
  'pdk_common',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'common'

common_files = [ 
  'coriolis/__init__.py',
  'coriolis/pnrcheck.py',
  'coriolis/s2r.py',
  'coriolis/scr.py',
  'coriolis/sta.py',
]

py.install_sources( files(common_files), subdir: 'pdks/common' )

# gf180mcu
install_subdir( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/cells'  , install_dir: pdks_dir / 'gf180mcu' )
install_subdir( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/models' , install_dir: pdks_dir / 'gf180mcu' )
install_subdir( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/rules'  , install_dir: pdks_dir / 'gf180mcu' )
install_subdir( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/tech'   , install_dir: pdks_dir / 'gf180mcu' )

py.install_sources( 'libs.tech/globalfoundries-pdk-libs-gf180mcu_fd_pr/LICENSE', subdir: 'pdks/common/gf180mcu' )

# ihp-sg13g3
install_subdir( 'libs.tech/IHP-Open-PDK/ihp-sg13g2/libs.qa'   , install_dir: pdks_dir / 'ihp-sg13g2' )
install_subdir( 'libs.tech/IHP-Open-PDK/ihp-sg13g2/libs.ref'  , install_dir: pdks_dir / 'ihp-sg13g2' )
install_subdir( 'libs.tech/IHP-Open-PDK/ihp-sg13g2/libs.tech' , install_dir: pdks_dir / 'ihp-sg13g2' )

py.install_sources( 'libs.tech/IHP-Open-PDK/LICENSE', subdir: 'pdks/common/ihp-sg13g2' )

# sky130
install_subdir( 'libs.tech/skywater-pdk-libs-sky130_fd_pr/cells'  , install_dir: pdks_dir / 'sky130' )
install_subdir( 'libs.tech/skywater-pdk-libs-sky130_fd_pr/models' , install_dir: pdks_dir / 'sky130' )
install_subdir( 'libs.tech/skywater-pdk-libs-sky130_fd_pr/tech'   , install_dir: pdks_dir / 'sky130' )

py.install_sources( 'libs.tech/skywater-pdk-libs-sky130_fd_pr/LICENSE', subdir: 'pdks/common/sky130' )
