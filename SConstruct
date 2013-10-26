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

env.Append(CPPPATH = './')

# py
dump_obj = env.Object('dump.cpp', CCFLAGS=ccflags)
pymodule_obj = env.Object('pymodule.cpp', CCFLAGS=ccflags)
	
# tuple
env.Program(['test/test_tuple.cpp', dump_obj], CCFLAGS=ccflags)

# pyresult
if not in_windows:
	env.Program('test/test_pyresult.cpp', CCFLAGS=ccflags)

# pyfunc
env.Program('test/test_pyfunc.cpp', CCFLAGS=ccflags)

# pymodule
env.Program(['test/test_pymodule.cpp', pymodule_obj], CCFLAGS=ccflags)

# py
env.Program(['test/test_py.cpp', pymodule_obj], CCFLAGS=ccflags)

# bench
env.Program(['test/bench_call.cpp', pymodule_obj], CCFLAGS=ccflags)

# dict
env.Program(['test/test_dict.cpp', dump_obj], CCFLAGS=ccflags)

# object
env.Program(['test/test_object.cpp', dump_obj], CCFLAGS=ccflags)

# native
env.Program(['test/test_native.cpp', 'native.cpp'], CCFLAGS=ccflags)

# develop veci
env.Program('embedding.cpp', CCFLAGS=ccflags)
env.Program('predef_aproach.cpp', CCFLAGS=ccflags)

#env.Program('rtaproach.cpp', CCFLAGS=ccflags)



