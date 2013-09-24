#include "dump.hpp"
#include <iostream>
using std::cout;


void dump_tuple(PyObject * tuple)
{
	assert(PyTuple_CheckExact(tuple)
		&& "logic-error: nie je tuple objektom");

	cout << "tuple:\n";

	for (int i = 0; i < PyTuple_Size(tuple); ++i)
	{
		PyObject * elem = PyTuple_GetItem(tuple, i);
		PyObject * repr = PyObject_Repr(elem);
		if (repr)
			cout << "  " << PyUnicode_AsUTF8(repr) << "\n";
		else
			cout << "  unknown object representation\n";
		Py_DECREF(repr);
	}
}
