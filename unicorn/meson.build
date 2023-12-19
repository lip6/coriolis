subdir('src')
subdir('python')
subdir('doc/unicorn')

Unicorn = declare_dependency(
  link_with: [unicorn],
  include_directories: include_directories('src'),
  dependencies: [Katana]
)

