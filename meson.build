project('bitwuzla',
  ['c', 'cpp'],
  meson_version: '>= 0.64',
  version: '1.0-dev',
  license: 'mit',
  default_options: [
    'c_std=c17',
    'cpp_std=c++17',
    'buildtype=release',
    'default_library=static',
    'strip=true'
  ]
)

# Build parameters
build_static = get_option('default_library') == 'static' and get_option('b_sanitize') == 'none'
enable_python = get_option('python')

# Configure testing
if get_option('testing').auto()
  if get_option('buildtype') == 'release'
    enable_testing = false
  elif get_option('buildtype') == 'debug'
    enable_testing = true
  endif
else
  enable_testing = get_option('testing').enabled()
endif

if get_option('unit-testing').auto()
  enable_unit_testing = enable_testing
else
  enable_unit_testing = get_option('unit-testing').enabled()
endif

# Note: For now we directly include bzla-libs until it gets merged to the repo.
subdir('lib/bzla-libs')

subdir('src')

if enable_python
  subdir('src/api/python')
endif

if enable_testing
  if enable_unit_testing
    subdir('test/unit')
  endif
  subdir('test/regress')
  if enable_python
    subdir('test/python')
  endif
endif

# Install public headers
install_subdir('include/bitwuzla', install_dir: 'include')