env = Environment()
env['CXX'] = 'clang++'

env.ParseConfig('pkg-config --cflags --libs python3')

ccflags = '-std=c++11 -O0 -g'

# pyresult
env.Program('test_pyresult.cpp', CCFLAGS=ccflags)

# tuple utils
env.Program('test_tuple.cpp', CCFLAGS=ccflags)

# pyfunc
env.Program('test_pyfunc.cpp', CCFLAGS=ccflags)

# pymodule
pymodule_obj = env.Object('pymodule.cpp', CCFLAGS=ccflags)
env.Program(['test_pymodule.cpp', pymodule_obj], CCFLAGS=ccflags)

# py
env.Program(['test_py.cpp', pymodule_obj], CCFLAGS=ccflags)

# bench
env.Program(['bench_call.cpp', pymodule_obj], CCFLAGS=ccflags)


# develop veci
env.Program('embedding.cpp', CCFLAGS=ccflags)
env.Program('predef_aproach.cpp', CCFLAGS=ccflags)

#env.Program('rtaproach.cpp', CCFLAGS=ccflags)
#dump_obj = env.Object('dump.cpp', CCFLAGS=ccflags)
