#include <vector>
#include <iostream>
#include "object.hpp"
#include "dump.hpp"

using std::vector;
using std::cout;


int main(int argc, char * argv[])
{
	Py_Initialize();

	{
	vector<int> v{1, 2, 3, 4, 5};
	PyObject * o = py::pythonize(py::as_tuple(v));
	cout << o << "\n";
	Py_DECREF(o);
	}

	{
	vector<int> v{1, 2, 3, 4, 5, 6};
	PyObject * o = py::pythonize(py::as_list(v));
	cout << o << "\n";
	Py_DECREF(o);
	}

	Py_Finalize();

	return 0;
}
