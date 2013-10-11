#include "py.hpp"
#include <iostream>

using std::cout;


void test_oneshot_call()
{
	double s;
	call("test", "sin", 30.0) >> s;
	cout << "sin(30.0):" << s << "\n";
}

void test_call_with_module()
{
	pymodule module("test");
	double s;
	call(module, "sin", 30.0) >> s;
	cout << "sin(30.0):" << s << "\n";
}

void test_call_with_function()
{
	pymodule module("test");
	pyfunc function = module("sin");
	double s;
	call(function, 30.0) >> s;
	cout << "sin(30.0):" << s << "\n";
}

void test_call_with_module_rvalue()
{
	double s;
	call(pymodule("test"), "sin", 30.0) >> s;
	cout << "sin(30.0):" << s << "\n";
}

void test_call_with_function_rvalue()
{
	pymodule math_module("test");
	double s;
	call(math_module("sin"), 30.0) >> s;
	cout << "sin(30.0):" << s << "\n";
}

int main(int argc, char * argv[])
{
	test_oneshot_call();
	test_call_with_module();
	test_call_with_function();
	test_call_with_module_rvalue();
	test_call_with_function_rvalue();
	return 0;
}
