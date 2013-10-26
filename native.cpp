#include "native.hpp"

namespace detail {

native_module::native_module(char const * module_name,  PyObject*(*initfunc)())
	: module(nullptr)
{
	PyImport_AppendInittab(module_name, initfunc);
}

native_module::~native_module()
{
	for (auto meth : _methods)
		delete meth;
}

bool native_module::append_method(char const * name, PyCFunction func)
{
	PyMethodDef * desc = new PyMethodDef;
	desc->ml_name = name;
	desc->ml_meth = func;
	desc->ml_flags = METH_VARARGS;
	desc->ml_doc = NULL;
	_methods.push_back(desc);

	PyObject * func_obj = PyCFunction_New(desc, NULL);
	if (!func_obj)
		return false;

	PyDict_SetItemString(PyModule_GetDict(module), name, func_obj);
	Py_DECREF(func_obj);

	return true;
}

}  // detail
