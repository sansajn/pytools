/* infrastruktura vyvinuta v tuple_helper.cpp */

#include <Python.h>
#include <list>
#include <vector>
#include <string>


// tuple low level manipulators for simple types
template <typename T>
void set_item(PyObject * tuple, size_t pos, T const & val);

template <>
void set_item<long>(PyObject * tuple, size_t pos, long const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyLong_FromLong(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);  // arg nemusim uvolnovat, lebo SetItem() spravi tuple vlastnikom
}

template <>
void set_item<int>(PyObject * tuple, size_t pos, int const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyLong_FromLong(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

template <>
void set_item<double>(PyObject * tuple, size_t pos, double const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyFloat_FromDouble(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

template <>
void set_item<float>(PyObject * tuple, size_t pos, float const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyFloat_FromDouble(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

template <>
void set_item<std::string>(PyObject * tuple, size_t pos, std::string const & val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val.c_str(), val.size(), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

// pretazenie pre c-stringy
void set_item(PyObject * tuple, size_t pos, char * val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyUnicode_DecodeUTF8(val, strlen(val), NULL);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
}

void set_item(PyObject * tuple, size_t pos, char const * val)
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
void set_item(PyObject * tuple, size_t pos, std::vector<T> const & val)
{
	PyObject * pycontainer = PyTuple_New(val.size());

	size_t counter = 0;
	for (auto & v : val)
		set_item(pycontainer, counter++, v);

	PyTuple_SetItem(tuple, pos, pycontainer);
}

template <typename T>
void set_item(PyObject * tuple, size_t pos, std::list<T> const & val)
{
	PyObject * pycontainer = PyTuple_New(val.size());

	size_t counter = 0;
	for (auto & v : val)
		set_item(pycontainer, counter++, v);

	PyTuple_SetItem(tuple, pos, pycontainer);
}

namespace Private {

void tuple_set_recursive(PyObject * tuple, size_t pos) {}

template <typename T, typename ... Tail>
void tuple_set_recursive(PyObject * tuple, size_t pos, T head, Tail ... tail)
{
	set_item(tuple, pos, head);
	tuple_set_recursive(tuple, pos+1, tail ...);
}

}  // Private

// umoznuje vkladanie lubovolneho poctu argumentou
template <typename T, typename ... Tail>
void tuple_set(PyObject * tuple, T head, Tail ... tail)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");
	Private::tuple_set_recursive(tuple, 0, head, tail ...);
}
