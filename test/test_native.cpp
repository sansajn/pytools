#include "native.hpp"
#include <list>
#include <string>
#include <iostream>

using std::list;
using std::string;


PyObject * test_greet(PyObject * self, PyObject * args)
{
	if (!PyArg_ParseTuple(args, ":greet"))
		return NULL;
	return PyUnicode_FromString("hello, native");
}

PYTHON_MODULE(test)
{
	if (!current_module())
		std::cout << "current_module() == nullptr\n";
	def("greet", test_greet);
}


void CAT(test_, cat)()
{
	std::cout << "hello from test_cat()\n";
}


int main(int argc, char * argv[])
{
	std::cout << STRINGIZE(ahoj) << "\n";
	test_cat();

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * py_module_name = PyUnicode_FromString("native");

	// importuje modul
	PyObject * py_module = PyImport_Import(py_module_name);

	Py_DECREF(py_module);
	Py_DECREF(py_module_name);

	Py_Finalize();

	return 0;
}
