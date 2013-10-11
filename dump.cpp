#include "dump.hpp"


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
