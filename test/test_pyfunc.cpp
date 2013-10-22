#include <Python.h>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <string>
#include "tuple.hpp"
#include "dump.hpp"
#include "pyfunc.hpp"

using std::cout;
using std::vector;
using std::string;

// tests
void simple_call();
void simple_call_with_arg();
void simple_call_with_args();
void complex_return_call();
void custom_arg();


int main(int argc, char * argv[])
{
	simple_call();
	simple_call_with_arg();
	simple_call_with_args();
	complex_return_call();
	custom_arg();
	return 0;	
}

void simple_call()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyfunc func(PyObject_GetAttrString(pModule, "simple_function"));
	func();
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void simple_call_with_arg()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyfunc func(PyObject_GetAttrString(pModule, "call_integer_func"));
	func(101);
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void simple_call_with_args()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyfunc func(PyObject_GetAttrString(pModule, "multiple_argument_call"));
	func(101, "1000001", 342.0f, 9829, 1234.0, 6780);
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void complex_return_call()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyfunc func(PyObject_GetAttrString(pModule, "complex_return"));
	string s;
	int i;
	vector<int> v;
	func() >> s >> i >> v;
	cout << "returned: " << s << ", " << i << ", (";
	for (auto & e : v)
		cout << e << ", ";
	cout << ")\n";
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void custom_arg()
{
}
