/* rutiny umoznujuce pracu s n-ticou */
#include <Python.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "tuple.hpp"

using std::string;
using std::vector;
using std::list;
using std::cout;

std::ostream & operator<<(std::ostream & out, PyObject * py)
{
	PyObject * repr = PyObject_Repr(py);
	if (repr)
		out << PyUnicode_AsUTF8(repr);
	else
		out << "unknown";
	Py_DECREF(repr);

	return out;
}

void test_tuple_set()
{
	Py_Initialize();

	PyObject * tuple = PyTuple_New(4);
	long i = 101, j = 1001, k = 10002, m = 100002;
	tuple_set(tuple, i, j, k, m);
	cout << tuple << "\n";
	Py_DECREF(tuple);

	PyObject * tuple2 = PyTuple_New(6);
	tuple_set(tuple2, 101, 1001, 10002.0, 100002, 3.0f, 12334);
	cout << tuple2 << "\n";
	Py_DECREF(tuple2);

	PyObject * tuple3 = PyTuple_New(3);
	string s = "hello";
	char * cstr = "c-string";  // fires warning
	tuple_set(tuple3, 1209, s, cstr);
	cout << tuple3 << "\n";
	Py_DECREF(tuple3);

	PyObject * tuple4 = PyTuple_New(3);
	tuple_set(tuple4, string("hello"), 9977, "hello-c-string");
	cout << tuple4 << "\n";
	Py_DECREF(tuple4);

	PyObject * tuple5 = PyTuple_New(3);
	vector<int> vec_data{100, 50, 25, 200, 400};
	tuple_set(tuple5, "begin", vec_data, "end");
	cout << tuple5 << "\n";
	Py_DECREF(tuple5);

	Py_Finalize();
}

int main(int argc, char * argv[])
{
	test_tuple_set();
	return 0;
}
