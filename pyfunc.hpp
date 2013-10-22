#pragma once
#include <Python.h>
#include <cassert>
#include "tuple.hpp"
#include "pyresult.hpp"

//! Python function abstraction.
class pyfunc
{
public:
	pyfunc() : _func(nullptr) {}

	pyfunc(PyObject * func)	 : _func(func)
	{}

	pyfunc(pyfunc &) = delete;
	pyfunc & operator=(pyfunc const &) = delete;

	pyfunc(pyfunc && func)	: _func(func._func)
	{
		func._func = nullptr;
	}

	~pyfunc()
	{
		if (_func)
			Py_DECREF(_func);
	}

	pyresult operator()()
	{
		assert(PyCallable_Check(_func)
			&& "logic-error: not callable python object");

		PyObject * result = PyObject_CallObject(_func, NULL);
		return pyresult(result);
	}

	template <typename T, typename ... Tail>
	pyresult operator()(T const & head, Tail const & ... tail)
	{
		assert(PyCallable_Check(_func)
			&& "logic-error: not callable python object");

		PyObject * pyargs = PyTuple_New(1 + sizeof ... (Tail));
		tuple_set(pyargs, head, tail ...);
		PyObject * result = PyObject_CallObject(_func, pyargs);
		Py_DECREF(pyargs);
		return pyresult(result);
	}

private:
	PyObject * _func;
};
