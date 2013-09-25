#include "pyfunc.hpp"

pyfunc::~pyfunc()
{
	if (_obj)
		Py_DECREF(_obj);
	_obj = nullptr;
}

pyfunc pyfunc::operator()()
{
	assert(PyCallable_Check(_obj)
		&& "logic-error: not callable python object");
	PyObject * args = PyTuple_New(0);
	PyObject * result = PyObject_CallObject(_obj, args);
	Py_DECREF(args);
	return pyfunc(result);
}

