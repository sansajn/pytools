#pragma once
#include <Python.h>
#include <string>
#include <vector>


inline void check_tuple_input(PyObject * obj, size_t pos)
{
	assert(PyTuple_Check(obj) && "python-tuple expected");
	assert(pos < PyTuple_Size(obj) && "tuple position ou of range");
}

template <typename R>
inline R get_item(PyObject * obj, size_t pos);

template <>
inline long get_item<long>(PyObject * obj, size_t pos)
{
	check_tuple_input(obj, pos);
	PyObject * item = PyTuple_GetItem(obj, pos);
	assert(PyLong_Check(item) && "python-long expected");
	return PyLong_AsLong(item);
}

template <>
inline int get_item<int>(PyObject * obj, size_t pos)
{
	return int(get_item<long>(obj, pos));
}

template <>
inline std::string get_item<std::string>(PyObject * obj, size_t pos)
{
	check_tuple_input(obj, pos);
	PyObject * item = PyTuple_GetItem(obj, pos);
	assert(PyUnicode_Check(item) && "python-unicode expected");
	return std::string(PyUnicode_AsUTF8(item));
}


class pyresult
{
public:
	pyresult(PyObject * result);
	~pyresult();

	template <typename T>
	pyresult & operator>>(T & val)
	{
		val = get_item<T>(_result, _pos++);
		return *this;
	}

	template <typename T>
	pyresult & operator>>(std::vector<T> & val)
	{
		PyObject * tuple = PyTuple_GetItem(_result, _pos++);
		assert(PyTuple_Check(tuple) && "python-tuple expected");
		for (int i = 0; i < PyTuple_Size(tuple); ++i)
			val.push_back(get_item<T>(tuple, i));
		return *this;
	}

/*
	template <typename Iterator>
	pyresult & copy_to(Iterator begin)
	{}
*/

private:
	int _pos;
	PyObject * _result;
};

