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

// custom type integration test
struct coordinate
{
	float lat, lon;
};

void tuple_at(PyObject * tuple, size_t pos, coordinate const & val)
{
	PyObject * pycoord = PyTuple_New(2);
	tuple_at(pycoord, 0, val.lat);
	tuple_at(pycoord, 1, val.lon);
	PyTuple_SetItem(tuple, pos, pycoord);
}

void test_custom_type()
{
	coordinate city_pos{14.2347, 48.1985};
	call("test", "print_coordinates", city_pos);

	// embedded call
	call("test", "print_coordinates", coordinate{15.2347, 49.1985});
}

int main(int argc, char * argv[])
{
	test_oneshot_call();
	test_call_with_module();
	test_call_with_function();
	test_call_with_module_rvalue();
	test_call_with_function_rvalue();
	test_custom_type();
	return 0;
}
