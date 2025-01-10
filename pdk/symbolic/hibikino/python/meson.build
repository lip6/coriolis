
install_path = 'coriolis/pdk/symbolic/hibikino'
patterns = [
  '*.py',
  'CATAL',
  '*.ap',
  '*.vbe'
]

# Install files using find, as Meson does not support glob pattern search
find_args = []
foreach pattern : patterns
  if find_args.length() > 1
    find_args += ['-o']
  endif
  find_args += ['-name', pattern]
endforeach

find_cmd = run_command(['find', '.'] + find_args, check: true)
files = find_cmd.stdout().strip().split('\n')

if files.length() > 0 and files[0] != ''
  py.install_sources(
    files,
    subdir: install_path,
    preserve_path: true   # Preserve the directory structure
  )
endif
