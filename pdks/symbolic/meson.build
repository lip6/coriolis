
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

py.install_sources('__init__.py', subdir: 'pdks/symbolic' )

install_subdir( 'lsxlib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'lsxlib'    )
install_subdir( 'niolib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'niolib'    )
install_subdir( 'nsxlib2/cells'   , strip_directory:true,   install_dir: pdks_dir / 'nsxlib2'   )
install_subdir( 'vsclib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'vsclib'    )
install_subdir( 'sxlib/cells'     , strip_directory:true,   install_dir: pdks_dir / 'sxlib'     )
install_subdir( 'hibikino/cells'  , strip_directory:true,   install_dir: pdks_dir / 'hibikino'  )
install_subdir( 'mpxlib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'mpxlib'    )
install_subdir( 'msplib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'msplib'    )
install_subdir( 'msxlib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'msxlib'    )
install_subdir( 'nramlib/cells'   , strip_directory:true,   install_dir: pdks_dir / 'nramlib'   )
install_subdir( 'nrf2lib/cells'   , strip_directory:true,   install_dir: pdks_dir / 'nrf2lib'   )
install_subdir( 'nrflib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'nrflib'    )
install_subdir( 'nsxlib/cells'    , strip_directory:true,   install_dir: pdks_dir / 'nsxlib'    )
install_subdir( 'phlib/cells'     , strip_directory:true,   install_dir: pdks_dir / 'phlib'     )
install_subdir( 'phlib80/cells'   , strip_directory:true,   install_dir: pdks_dir / 'phlib80'   )
