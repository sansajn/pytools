/* rutiny umoznujuce pracu s n-ticou */
#include <Python.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "tuple.hpp"
#include "dump.hpp"

using std::string;
using std::vector;
using std::list;
using std::cout;


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

void test_tuple_fill()
{
	Py_Initialize();

	py::tuple t(3);
	t << 101 << 101.101 << std::string("102");
	cout << t.native() << "\n";

	py::tuple u(3);
	u << 111 << (py::tuple(2) << 17.32 << 49.48) << 112;
	cout << u.native() << "\n";

	Py_Finalize();
}

void tuple_read()
{
	Py_Initialize();

	py::tuple t(3);
	t << 101 << 101.101 << std::string("102");

	int i;
	double d;
	std::string s;
	t >> i >> d >> s;

	assert(i == 101 && d == 101.101 && s == "102"	&& "ziskane data su nekorektne");

	cout << "i:" << i << ", d:" << d << ", s:" << s << "\n";
}

int main(int argc, char * argv[])
{
	test_tuple_set();
	test_tuple_fill();
	tuple_read();
	return 0;
}
