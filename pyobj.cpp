#include <Python.h>
#include <cassert>
#include <iostream>

using std::cout;


// tuple low level manipulators
template <typename T>
void set_item(PyObject * obj, T const & x, size_t pos);

template <>
void set_item<int>(PyObject * obj, int const & val, size_t pos)
{
	assert(PyTuple_CheckExact(obj) &&
		"logic-error: not created as tuple object");

	PyObject * arg = PyLong_FromLong(val);
	PyTuple_SetItem(obj, pos, arg);
	// arg nemusim uvolnovat, lebo SetItem je vlastnikom
}


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

	pyobj operator()(int const & x) {
		assert(PyCallable_Check(_obj)
			&& "logic-error: not callable python object");
		PyObject * args = PyTuple_New(1);
		set_item(args, x, 0);
		PyObject * result = PyObject_CallObject(_obj, args);
		Py_DECREF(args);
		return pyobj(result);
	}

private:
	PyObject * _obj;
};


void simple_call();
void simple_call_with_arg();


int main(int argc, char * argv[])
{
	simple_call();
	simple_call_with_arg();
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
