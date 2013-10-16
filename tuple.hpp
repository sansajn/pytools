/* umoznuje plnit n-ticu volanim metody tuple_set(...) */
#pragma once
#include <Python.h>
#include <list>
#include <vector>
#include <string>


// tuple low level manipulators for simple types
template <typename T>
inline void tuple_at(PyObject * tuple, size_t pos, T const & val);

template <>
inline void tuple_at<PyObject *>(PyObject * tuple, size_t pos, PyObject * const & val)
{
	assert(PyTuple_CheckExact(tuple)
		&& "logic-error: python tuple object expected");

	PyTuple_SetItem(tuple, pos, val);
}

template <>
inline void tuple_at<long>(PyObject * tuple, size_t pos, long const & val)
{
	assert(PyTuple_CheckExact(tuple)
		&&	"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyLong_FromLong(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);  // arg nemusim uvolnovat, lebo SetItem() spravi tuple vlastnikom
}

template <>
inline void tuple_at<int>(PyObject * tuple, size_t pos, int const & val)
{
	tuple_at<long>(tuple, pos, val);
}

template <>
inline void tuple_at<double>(PyObject * tuple, size_t pos, double const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyFloat_FromDouble(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

template <>
inline void tuple_at<float>(PyObject * tuple, size_t pos, float const & val)
{
	tuple_at<double>(tuple, pos, val);
}

template <>
inline void tuple_at<std::string>(PyObject * tuple, size_t pos, std::string const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val.c_str(), val.size(), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

// pretazenie pre c-stringy
inline void tuple_at(PyObject * tuple, size_t pos, char * val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val, strlen(val), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

inline void tuple_at(PyObject * tuple, size_t pos, char const * val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val, strlen(val), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}


//! \note toto zjednot pod hlavickou iterable (zatial netusim ako)

// toto je overloading (podpora pre kontajnery)
template <typename T>
inline void tuple_at(PyObject * tuple, size_t pos, std::vector<T> const & val)
{
	PyObject * pycontainer = PyTuple_New(val.size());

	size_t counter = 0;
	for (auto & v : val)
		tuple_at(pycontainer, counter++, v);

	PyTuple_SetItem(tuple, pos, pycontainer);
}

template <typename T>
inline void tuple_at(PyObject * tuple, size_t pos, std::list<T> const & val)
{
	PyObject * pycontainer = PyTuple_New(val.size());

	size_t counter = 0;
	for (auto & v : val)
		tuple_at(pycontainer, counter++, v);

	PyTuple_SetItem(tuple, pos, pycontainer);
}

namespace Private {

inline void tuple_set_recursive(PyObject * tuple, size_t pos) {}

template <typename T, typename ... Tail>
inline void tuple_set_recursive(PyObject * tuple, size_t pos, T head, Tail ... tail)
{
	tuple_at(tuple, pos, head);
	tuple_set_recursive(tuple, pos+1, tail ...);
}

}  // Private

// umoznuje vkladanie lubovolneho poctu argumentou
template <typename T, typename ... Tail>
inline void tuple_set(PyObject * tuple, T head, Tail ... tail)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");
	Private::tuple_set_recursive(tuple, 0, head, tail ...);
}

namespace py {

class tuple
{
public:
	tuple(size_t elems)
		: _pos(0)
	{
		_obj = PyTuple_New(elems);
	}

	template <typename T>
	tuple & operator<<(T const & rhs)
	{
		assert(_pos < PyTuple_Size(_obj)
			&& "logic-error: out of range (no space for another element)");
		tuple_at<T>(_obj, _pos++, rhs);
		return *this;
	}

	PyObject * native() const	{return _obj;}

	// begin()
	// end()

private:
	size_t _pos;
	PyObject * _obj;
};

template <typename T>
inline tuple to_tuple(std::vector<T> const & val)
{
	tuple t(val.size());
	for (auto & v : val)
		t << v;
}

};  // py

template <>
inline void tuple_at<py::tuple>(PyObject * tuple, size_t pos, py::tuple const & val)
{
	PyTuple_SetItem(tuple, pos, val.native());
}
