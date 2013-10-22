#include <cmath>
#include <chrono>
#include <iostream>
#include "py.hpp"

using std::cout;

namespace chrono = std::chrono;
typedef chrono::high_resolution_clock chrono_clock;

void bench_simple_call();
void bench_call();
void bench_native_call();


int main(int argc, char * argv[])
{
	bench_simple_call();
	bench_call();
	bench_native_call();
	return 0;
}

void bench_simple_call()
{
	chrono_clock::time_point start_tm = chrono_clock::now();

	double s;
	for (int i = 0; i < 1000; ++i)
		call("test", "sin", i*(360.0/1000.0)) >> s;

	chrono_clock::duration dt = chrono_clock::now() - start_tm;

	long count = std::chrono::duration_cast<chrono::milliseconds>(dt).count();
	cout << "simple-calls:" << count << "ms, one-simple-call:" << count/1000.0 << "ms\n";
}

void bench_call()
{
	chrono_clock::time_point start_tm = chrono_clock::now();

	pymodule math_module("test");
	pyfunc func = math_module("sin");
	double s;
	for (int i = 0; i < 1000; ++i)
		call(func, i*(360.0/10000.0)) >> s;

	chrono_clock::duration dt = chrono_clock::now() - start_tm;

	long count = chrono::duration_cast<chrono::milliseconds>(dt).count();
	cout << "calls:" << count << "ms, one-call:" << count/1000.0 << "ms\n";
}

void bench_native_call()
{
	chrono_clock::time_point start_tm = chrono_clock::now();

	double const PI = 3.14159265;

	double s;
	for (int i = 0; i < 1000; ++i)
		s = sin((i*(360.0/10000.0))*(PI/180.0));

	chrono_clock::duration dt = chrono_clock::now() - start_tm;

	long count = chrono::duration_cast<chrono::nanoseconds>(dt).count();
	cout << "native-calls:" << count << "ns, one-native-call:" << count/1000.0 << "ns\n";
}


