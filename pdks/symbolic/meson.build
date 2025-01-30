
project(
  'pdk_symbolic_cells',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'symbolic' / 'cells'

install_subdir( 'lsxlib/cells'  , install_dir: pdks_dir / 'lsxlib' )
install_subdir( 'nsxlib2/cells' , install_dir: pdks_dir / 'nsxlib2' )
install_subdir( 'vsclib/cells' , install_dir: pdks_dir / 'vsc' )
