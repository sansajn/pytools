#include "pyfunc.hpp"

#include <iostream>
using std::cout;


pyfunc::~pyfunc()
{
	if (_obj)
		Py_DECREF(_obj);
	_obj = nullptr;
}

pyresult pyfunc::operator()()
{
	assert(PyCallable_Check(_obj)
		&& "logic-error: not callable python object");
	PyObject * result = PyObject_CallObject(_obj, NULL);

	// co to je ???
	if (result == Py_None)
	{
		int dummy = 101;
	}
	else if (PyType_Check(result))
	{
		int dummy = 101;
	}
	else if (PyLong_Check(result))
	{
		int dummy = 101;
	}
	else if (PyBool_Check(result))
	{
		int dummy = 101;
	}
	else if (PyFloat_Check(result))
	{
		int dummy = 101;
	}
	else if (PyComplex_Check(result))
	{
		int dummy = 101;
	}
	else if (PyBytes_Check(result))
	{
		int dummy = 101;
	}
	else if (PyByteArray_Check(result))
	{
		int dummy = 101;
	}
	else if (PyUnicode_Check(result))
	{
		int dummy = 101;
	}
	else if (PyTuple_Check(result))
	{
		int dummy = 101;
	}
	else if (PyList_Check(result))
	{
		int dummy = 101;
	}
	else if (PyDict_Check(result))
	{
		int dummy = 101;
	}
	else if (PySet_Check(result))
	{
		int dummy = 101;
	}

	return pyresult(result);
}
