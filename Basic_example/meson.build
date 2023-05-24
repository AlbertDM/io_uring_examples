project('io_uring-basic_example', 'c')

uring_dep = dependency('liburing', version : '>= 0.1.0')

executable('basic_example_meson',
  sources: 'basic_example.c',
  dependencies: uring_dep
)
