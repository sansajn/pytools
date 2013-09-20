#include <Python.h>
#include <cassert>
#include <iostream>

using std::cout;

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

private:
	PyObject * _obj;
};


void simple_call();


int main(int argc, char * argv[])
{
	simple_call();
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

