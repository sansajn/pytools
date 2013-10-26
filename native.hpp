/*! Podpora pre vytvaranie modulou a pridavanie metod, nasledujuca ukazka
vytvori modul 'test' a prida metodu 'greet' [Example:

	PyObject * test_greet(PyObject * self, PyObject * args)	 {
		return PyUnicode_FromString("hello native!");
	}

	PYTHON_MODULE(test) {
		def("greet", test_greet);
	}

--- end example]. V pythone sa tento modul da pouzit takto [Ukazka:

	>>> import test
	>>> test.greet()
	'hello native!'

--- koniec ukazky]. */

#pragma once
#include <Python.h>
#include <list>

namespace detail {

class native_module  //! toto by mohol byt scope, ktory si naplnim
{
public:
	native_module(char const * module_name,  PyObject*(*initfunc)());
	~native_module();
	PyObject * current_module() const {return module;}
	bool append_method(char const * name, PyCFunction func);

	PyObject * module;  // skutocna hodnota sa inicializuje v module-init

private:
	std::list<PyMethodDef *> _methods;
};

}  // detail

#define CAT(a, b) a ## b
#define STRINGIZE(name) #name

#define PYTHON_MODULE(name) \
	static PyMethodDef CAT(name,_methods)[] = { \
		{NULL, NULL, 0, NULL} \
	}; \
	static PyModuleDef CAT(name,_module) = { \
		PyModuleDef_HEAD_INIT, STRINGIZE(name), NULL, -1, CAT(name,_methods), \
		NULL, NULL, NULL, NULL \
	}; \
	static void CAT(name,_main)(); \
	static PyObject * CAT(PyInit_,name)(); \
	static detail::native_module name(STRINGIZE(name), CAT(PyInit_,name)); \
	static PyObject * current_module() {return name.current_module();} \
	static bool def(char const * method_name, PyCFunction impl) { \
		return name.append_method(method_name, impl); \
	} \
	PyObject * CAT(PyInit_,name)() {\
		name.module = PyModule_Create(&CAT(name,_module)); \
		CAT(name,_main)(); \
		return name.module; \
	} \
	void CAT(name,_main)()
