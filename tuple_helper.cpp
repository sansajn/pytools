/* rutiny umoznujuce pracu s n-ticou */
#include <Python.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "dump.hpp"

using std::string;
using std::vector;
using std::list;
using std::cout;


// tuple low level manipulators
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
void set_item<string>(PyObject * tuple, size_t pos, string const & val)
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
void set_item(PyObject * tuple, size_t pos, vector<T> const & val)
{
	PyObject * pycontainer = PyTuple_New(val.size());

	size_t counter = 0;
	for (auto & v : val)
		set_item(pycontainer, counter++, v);

	PyTuple_SetItem(tuple, pos, pycontainer);
}

template <typename T>
void set_item(PyObject * tuple, size_t pos, list<T> const & val)
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

void test_tuple_set()
{
	Py_Initialize();

	PyObject * tuple = PyTuple_New(4);
	long i = 101, j = 1001, k = 10002, m = 100002;
	tuple_set(tuple, i, j, k, m);
	dump_tuple(tuple);
	Py_DECREF(tuple);

	PyObject * tuple2 = PyTuple_New(6);
	tuple_set(tuple2, 101, 1001, 10002.0, 100002, 3.0f, 12334);
	dump_tuple(tuple2);
	Py_DECREF(tuple2);

	PyObject * tuple3 = PyTuple_New(3);
	string s = "hello";
	char * cstr = "c-string";  // fires warning
	tuple_set(tuple3, 1209, s, cstr);
	dump_tuple(tuple3);
	Py_DECREF(tuple3);

	PyObject * tuple4 = PyTuple_New(3);
	tuple_set(tuple4, string("hello"), 9977, "hello-c-string");
	dump_tuple(tuple4);
	Py_DECREF(tuple4);

	PyObject * tuple5 = PyTuple_New(3);
	vector<int> vec_data{100, 50, 25, 200, 400};
	tuple_set(tuple5, "begin", vec_data, "end");
	dump_tuple(tuple5);
	Py_DECREF(tuple5);

	Py_Finalize();
}

int main(int argc, char * argv[])
{
	test_tuple_set();
	return 0;
}
