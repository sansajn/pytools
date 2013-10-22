#include <iostream>
#include "pymodule.hpp"

using std::cout;


void test_simple_module()
{
	pymodule module("test");
	pyfunc func = module("simple_function");
	func();
}

void test_simple_return()
{
	pymodule module("test");
	pyfunc func = module("simple_return");
	int i;
	func() >> i;
	cout << "i:" << i << "\n";
}

int main(int argc, char * argv[])
{
	test_simple_module();
	test_simple_return();
	return 0;
}
