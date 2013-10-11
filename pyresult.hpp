/*! Implments function call result. */
#pragma once
#include <Python.h>
#include <string>
#include <vector>

//! python elementary types support
//@{
template <typename R>
inline R get(PyObject * o);

template <>
inline long get<long>(PyObject * o)
{
	assert(PyLong_Check(o) && "python long object expected");
	return PyLong_AsLong(o);
}

template <>
inline int get<int>(PyObject * o)
{
	return static_cast<int>(get<long>(o));
}

template <>
inline double get<double>(PyObject * o)
{
	assert(PyFloat_Check(o) && "python float object expected");
	return PyFloat_AsDouble(o);
}

template <>
inline float get<float>(PyObject * o)
{
	return static_cast<float>(get<double>(o));
}

template <>
inline std::string get<std::string>(PyObject * o)
{
	assert(PyUnicode_Check(o) && "python unicode object expected");
	return PyUnicode_AsUTF8(o);
}

struct generic_object_holder
{
	generic_object_holder(PyObject * o) : _o(o) {}
	~generic_object_holder() {Py_DECREF(_o);}
	operator PyObject*() {return _o;}
	PyObject * _o;
};

//! Python function result abstraction.
template <typename R>
inline R sequence_at(PyObject * o, size_t pos)
{
	assert(PySequence_Check(o) 
		&& "python object doesn't implement sequence protocol");
	assert(pos < PySequence_Size(o) && "position out of range");
	generic_object_holder item(PySequence_GetItem(o, pos));
	return get<R>(item);
}
//@}

class pyresult
{
public:
	pyresult(PyObject * o)
		: _pos(0), _result(o) 
	{}

	~pyresult()
	{
		if (_result)
			Py_DECREF(_result);
	}

	pyresult(pyresult const & rhs)
		: _pos(rhs._pos), _result(rhs._result)
	{
		Py_INCREF(_result);
	}

	pyresult(pyresult && rhs)
		: _pos(rhs._pos), _result(rhs._result)
	{
		rhs._result = nullptr;
	}

	template <typename T>
	pyresult & operator>>(T & val)
	{
		if (!PySequence_Check(_result))
		{
			val = get<T>(_result);
			assert(_pos++ == 0 && "viacnasobne citanie jednoduchej hodnoty");
		}
		else
			val = sequence_at<T>(_result, _pos++);

		return *this;
	}

	template <typename T>
	pyresult & operator>>(std::vector<T> & val)
	{
		PyObject * item = nullptr;
		if (_pos == 0)
			item = _result;
		else
			item = PySequence_GetItem(_result, _pos);

		if (!PySequence_Check(item))
			val.push_back(get<T>(item));
		else
			for (size_t i = 0; i < PySequence_Size(item); ++i)
				val.push_back(sequence_at<T>(item, i));

		if (item != _result)
			Py_DECREF(item);

		_pos += 1;

		return *this;
	}

private:
	size_t _pos;
	PyObject * _result;
};

//! \task: implementuj operator>>() pre list mimo classu

