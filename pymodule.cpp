#include "pymodule.hpp"

using std::string;

int pymodule::module_counter = 0;


pymodule::pymodule(string const & module)
{
	if (pymodule::module_counter == 0)
		Py_Initialize();

	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * py_name = PyUnicode_FromString(module.c_str());
	_module = PyImport_Import(py_name);

	assert(_module && "logic-error: python import module failed");

	Py_DECREF(py_name);

	pymodule::module_counter += 1;
}

pymodule::~pymodule()
{	
	if (_module)
		Py_DECREF(_module);

	pymodule::module_counter -= 1;
	if (pymodule::module_counter == 0)
		Py_Finalize();
}

pyfunc pymodule::function(string const & name)
{
	return pyfunc(PyObject_GetAttrString(_module, name.c_str()));
}
