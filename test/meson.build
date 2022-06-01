fmt_dep = dependency('fmt')
test_exe = executable('test.out', 'test_all.cpp', 'test_arithmetic.cpp', 'test_ctor.cpp', include_directories : inc, dependencies : fmt_dep)
test('fixed point library test', test_exe)
