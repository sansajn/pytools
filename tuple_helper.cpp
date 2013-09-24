#include <Python.h>
#include <iostream>
#include "dump.hpp"

using std::cout;


// tuple low level manipulators
template <typename T>
void set_item(PyObject * tuple, size_t pos, T val);

template <>
void set_item<long>(PyObject * tuple, size_t pos, long val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyLong_FromLong(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);  // arg nemusim uvolnovat, lebo SetItem() spravi tuple vlastnikom
}

template <>
void set_item<int>(PyObject * tuple, size_t pos, int val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyLong_FromLong(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);  // arg nemusim uvolnovat, lebo SetItem() spravi tuple vlastnikom
}

template <>
void set_item<double>(PyObject * tuple, size_t pos, double val)
{
	assert(PyTuple_CheckExact(tuple) &&
		"logic-error: not created as a tuple object");

	assert(pos < PyTuple_Size(tuple) && "logic-error: out of tuple range");

	PyObject * arg = PyFloat_FromDouble(val);

	assert(arg && "logic-error: nepodarila sa konverzia na python objekt");

	PyTuple_SetItem(tuple, pos, arg);
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
	tuple_set(tuple2, 101, 1001, 10002, 100002, 3, 12334);
	dump_tuple(tuple2);
	Py_DECREF(tuple2);

	Py_Finalize();
}

int main(int argc, char * argv[])
{
	test_tuple_set();
	return 0;
}
