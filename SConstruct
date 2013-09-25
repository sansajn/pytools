env = Environment()
env['CXX'] = 'clang++'

env.ParseConfig('pkg-config --cflags --libs python3')

ccflags = '-std=c++11 -O0 -g'
objs = env.Object(['dump.cpp', 'pyfunc.cpp'], CCFLAGS=ccflags)

env.Program('embedding.cpp', CCFLAGS='-std=c++11 -O0 -g')

env.Program(['test_tuple.cpp', objs], CCFLAGS=ccflags)
env.Program(['test_pyfunc.cpp', objs], CCFLAGS=ccflags)

env.Program('devel_pyresult.cpp', CCFLAGS=ccflags)
