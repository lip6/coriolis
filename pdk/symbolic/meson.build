project(
  'pdk_symbolic',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

pdks_dir = py.get_install_dir() / 'pdks' / 'symbolic'

patterns = [
  '*.py',
  'CATAL',
  '*.lib',
  '*.ap',
  '*.vbe',
  '*.gds'
]

# Install files using find, as Meson does not support glob pattern search
find_args = []
foreach pattern : patterns
  if find_args.length() > 1
    find_args += ['-o']
  endif
  find_args += ['-name', pattern]
endforeach

find_cmd = run_command(['find', 'libs.ref'] + find_args, check: true)
cells_files = find_cmd.stdout().strip().split('\n')

py.install_sources( files(cells_files), subdir: 'pdks/symbolic' , preserve_path: true )

tech_files = [ 
  'libs.tech/coriolis/symbolic/__init__.py'
]

py.install_sources( files(tech_files), subdir: 'pdks/symbolic' )

install_subdir( 'libs.ref'  , install_dir: pdks_dir )
install_subdir( 'libs.tech' , install_dir: pdks_dir , exclude_directories: ['coriolis']  )
