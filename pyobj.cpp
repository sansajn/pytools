#include <Python.h>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <vector>
#include "tuple.hpp"
#include "dump.hpp"

using std::cout;
using std::vector;

// toto je objekt s ktorým pracujem ako s funkciou, zvažiť
// premenovanie na pyfunc
class pyobj
{
public:
	pyobj() : _obj(nullptr) {}
	pyobj(PyObject * obj) : _obj(obj) {}

	~pyobj() {
		if (_obj)
			Py_DECREF(_obj);
		_obj = nullptr;
	}

	pyobj operator()() {
		assert(PyCallable_Check(_obj) 
			&& "logic-error: not callable python object");
		PyObject * args = PyTuple_New(0);
		PyObject * result = PyObject_CallObject(_obj, args);
		Py_DECREF(args);
		return pyobj(result);
	}

	template <typename T, typename ... Args>
	pyobj operator()(T head, Args ... args)	{
		assert(PyCallable_Check(_obj)
			&& "logic-error: not callable python object");
		PyObject * pyargs = PyTuple_New(1 + sizeof ... (Args));
		tuple_set(pyargs, head, args ...);
		PyObject * result = PyObject_CallObject(_obj, pyargs);
		Py_DECREF(pyargs);
		return pyobj(result);
	}

private:
	PyObject * _obj;
};


void simple_call();
void simple_call_with_arg();
void simple_call_with_args();


int main(int argc, char * argv[])
{
	simple_call();
	simple_call_with_arg();
	simple_call_with_args();
	return 0;	
}

void simple_call()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyobj func(PyObject_GetAttrString(pModule, "simple_function"));
	func();
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void simple_call_with_arg()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyobj func(PyObject_GetAttrString(pModule, "call_integer_func"));
	func(101);
	}

	Py_DECREF(pModule);

	Py_Finalize();
}

void simple_call_with_args()
{
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);

	{
	pyobj func(PyObject_GetAttrString(pModule, "multiple_argument_call"));
	func(101, "1000001", 342.0f, 9829, 1234.0, 6780);
	}

	Py_DECREF(pModule);

	Py_Finalize();
}
