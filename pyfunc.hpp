#pragma once
#include <Python.h>
#include <cassert>

class pyfunc
{
public:
	pyfunc() : _obj(nullptr) {}
	pyfunc(PyObject * obj) : _obj(obj) {}
	~pyfunc();

	pyfunc operator()();

	template <typename T, typename ... Tail>
	pyfunc operator()(T const & head, Tail const & ... tail) {
		assert(PyCallable_Check(_obj)
			&& "logic-error: not callable python object");
		PyObject * pyargs = PyTuple_New(1 + sizeof ... (Tail));
		tuple_set(pyargs, head, tail ...);
		PyObject * result = PyObject_CallObject(_obj, pyargs);
		Py_DECREF(pyargs);
		return pyfunc(result);
	}

private:
	PyObject * _obj;
};

