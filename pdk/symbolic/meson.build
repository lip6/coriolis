project(
  'pdk_symbolic',
  version: '0.1.0',
  meson_version: '>= 1.2.0',
  default_options: ['warning_level=3']
)

python = import('python')
py = python.find_installation('python3')
py_deps = py.dependency()

install_path = 'pdks/symbolic'
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
files = find_cmd.stdout().strip().split('\n')

if files.length() > 0 and files[0] != ''
  py.install_sources(
    files,
    subdir: install_path,
    preserve_path: true   # Preserve the directory structure
  )
endif

# subdir('sxlib/python')
# subdir('niolib/python')
# subdir('hibikino/python')
# subdir('lsxlib/python')
# subdir('mpxlib/python')
# subdir('msplib/python')
# subdir('msxlib/python')
# subdir('nramlib/python')
# subdir('nrf2lib/python')
# subdir('nrflib/python')
# subdir('nsxlib/python')
# subdir('nsxlib2/python')
# subdir('phlib/python')
# subdir('phlib80/python')
# subdir('sky130/python')
# subdir('vsclib/python')
