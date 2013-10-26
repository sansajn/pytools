/* umoznuje plnit n-ticu volanim metody tuple_set(...) */
#pragma once
#include <Python.h>
#include <list>
#include <vector>
#include <string>


// tuple low level manipulators for simple types
template <typename T>
inline void tuple_at(PyObject * tuple, size_t pos, T const & val);

template <typename R>
inline R tuple_at(PyObject * tuple, size_t pos);

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
inline long tuple_at<long>(PyObject * tuple, size_t pos)
{
	PyObject * o = PyTuple_GetItem(tuple, pos);
	assert(PyLong_Check(o) && "objekt na pozicii nie je typu long");
	return PyLong_AsLong(o);
}

template <>
inline void tuple_at<int>(PyObject * tuple, size_t pos, int const & val)
{
	tuple_at<long>(tuple, pos, val);
}

template <>
inline int tuple_at<int>(PyObject * tuple, size_t pos)
{
	return static_cast<int>(tuple_at<long>(tuple, pos));
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
inline double tuple_at<double>(PyObject * tuple, size_t pos)
{
	PyObject * o = PyTuple_GetItem(tuple, pos);
	assert(PyFloat_Check(o) && "objekt na pozicii nie je typu float");
	return PyFloat_AsDouble(o);
}

template <>
inline void tuple_at<float>(PyObject * tuple, size_t pos, float const & val)
{
	tuple_at<double>(tuple, pos, val);
}

template <>
inline float tuple_at<float>(PyObject * tuple, size_t pos)
{
	return static_cast<float>(tuple_at<double>(tuple, pos));
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

template <>
inline std::string tuple_at<std::string>(PyObject * tuple, size_t pos)
{
	PyObject * o = PyTuple_GetItem(tuple, pos);
	assert(PyUnicode_Check(o) && "objekt na pozicii nie je typu unicode");
	return std::string(PyUnicode_AsUTF8(o));
}

// pretazenie pre c-stringy
// TODO: toto sa da spravit ako pretzenie sablon (pozri result/pythonize)
inline void tuple_at(PyObject * tuple, size_t pos, char const * val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val, strlen(val), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

inline void tuple_at(PyObject * tuple, size_t pos, char * val)
{
	tuple_at(tuple, pos, const_cast<char const *>(val));
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

// tuple set implementation

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

//! Tuple abstraction.
class tuple
{
public:
	tuple(size_t elems)
		: _gpos(0), _ppos(0)
	{
		_obj = PyTuple_New(elems);
	}

	tuple(PyObject * obj)
		: _gpos(0), _ppos(0)
	{
		_obj = obj;
		Py_INCREF(_obj);
		assert(obj && "logic-error: nulovy odkaz na objekt");
	}

	~tuple() {Py_DECREF(_obj);}

	template <typename T>
	tuple & operator<<(T const & rhs)
	{
		assert(_gpos < PyTuple_Size(_obj)
			&& "logic-error: out of range (no space for another element)");
		tuple_at<T>(_obj, _gpos++, rhs);
		return *this;
	}

	template <typename T>
	tuple & operator>>(T & rhs)
	{
		assert(_ppos < PyTuple_Size(_obj)
			&& "logic-error: out of range (there isn't another element)");
		rhs = tuple_at<T>(_obj, _ppos++);
		return *this;
	}

	PyObject * native() const	{return _obj;}

private:
	size_t _gpos;  // get-pos
	size_t _ppos;  // set-pos
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
	Py_INCREF(val.native());
	PyTuple_SetItem(tuple, pos, val.native());
}
