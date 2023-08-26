subdir('src')
subdir('python')

Unicorn = declare_dependency(
  link_with: [unicorn],
  include_directories: include_directories('src'),
  dependencies: [Katana]
)

