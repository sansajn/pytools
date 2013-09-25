/* implementuje mechanizmus previazania vystupu */
#include <Python.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "pyresult.hpp"

using std::string;
using std::vector;
using std::cout;

void test_complex_return()
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

	PyObject * func = PyObject_GetAttrString(pModule, "complex_return");
	PyObject * args = PyTuple_New(0);

	pyresult result(PyObject_CallObject(func, args));

	string res_s;
	int res_i = -1;
	vector<int> res_v;
	result >> res_s >> res_i >> res_v;

	cout << res_s << ", " << res_i << ", (";
	for (auto & e : res_v)
		cout << e << ",";
	cout << ")\n";

	Py_DECREF(args);
	Py_DECREF(pModule);

	Py_Finalize();
}



int main(int argc, char * argv[])
{
	test_complex_return();
	return 0;
}
