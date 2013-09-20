env = Environment()
env.ParseConfig('pkg-config --cflags --libs python3')
env.Program('embedding.cpp', CCFLAGS='-std=c++11 -O0 -g')
env.Program('pyobj.cpp', CCFLAGS='-std=c++11 -O0 -g')
