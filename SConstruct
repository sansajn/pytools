env = Environment()
env['CXX'] = 'clang++'

env.ParseConfig('pkg-config --cflags --libs python3')

ccflags = '-std=c++11 -O0 -g'
objs = env.Object('dump.cpp', CCFLAGS=ccflags)

env.Program('embedding.cpp', CCFLAGS='-std=c++11 -O0 -g')

env.Program(['tuple_helper.cpp', objs], CCFLAGS=ccflags)
env.Program(['pyobj.cpp', objs], CCFLAGS=ccflags)
