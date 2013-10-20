import sys

env = Environment()

in_windows = sys.platform == 'win32'

if in_windows:
	ccflags = '/EHsc'
	env.Append(CPPPATH = ['C:\\Python33\\include'])
	env.Append(LIBPATH = ['C:\\Python33\\libs'])
	env.Append(LIBS = ['python33.lib'])
else:
	env['CXX'] = 'clang++'
	ccflags = '-std=c++11 -O0 -g'
	env.ParseConfig('pkg-config --cflags --libs python3')

	
# pyresult
if not in_windows:
	env.Program('test_pyresult.cpp', CCFLAGS=ccflags)

dump_obj = env.Object('dump.cpp', CCFLAGS=ccflags)

# tuple
env.Program(['test_tuple.cpp', dump_obj], CCFLAGS=ccflags)

# pyfunc
env.Program('test_pyfunc.cpp', CCFLAGS=ccflags)

# pymodule
pymodule_obj = env.Object('pymodule.cpp', CCFLAGS=ccflags)
env.Program(['test_pymodule.cpp', pymodule_obj], CCFLAGS=ccflags)

# py
env.Program(['test_py.cpp', pymodule_obj], CCFLAGS=ccflags)

# bench
env.Program(['bench_call.cpp', pymodule_obj], CCFLAGS=ccflags)

# dict
env.Program(['test_dict.cpp', dump_obj], CCFLAGS=ccflags)

# object
env.Program(['test_object.cpp', dump_obj], CCFLAGS=ccflags)

# develop veci
env.Program('embedding.cpp', CCFLAGS=ccflags)
env.Program('predef_aproach.cpp', CCFLAGS=ccflags)

#env.Program('rtaproach.cpp', CCFLAGS=ccflags)



