examples = [
  'checksatassuming',
  'options',
  'parse',
  'pushpop',
  'quickstart',
  'reset',
  'reset_assertions',
  'terminator',
  'unsatassumptions',
  'unsatcore',
]

bitwuzla_dep = dependency('bitwuzla')
foreach e : examples
  exe = executable(
    e, ''.join(e, '.cpp'),
    dependencies: [bitwuzla_dep])
    test(e, exe)
endforeach
