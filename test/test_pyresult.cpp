/* implementuje mechanizmus previazania vystupu */
#include <Python.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "pyresult.hpp"

#define BOOST_TEST_MODULE pyresult_test 
#include <boost/test/included/unit_test.hpp>

using std::string;
using std::vector;
using std::cout;

PyObject * load_module();
void unload_module(PyObject * module);

BOOST_AUTO_TEST_CASE(list_return)
{
	PyObject * module = load_module();
	PyObject * func = PyObject_GetAttrString(module, "list_return");

	{
	pyresult result(PyObject_CallObject(func, NULL));
	long n;
	double d;
	result >> n >> d;
	
	BOOST_CHECK_EQUAL(n, 101);
	BOOST_CHECK_EQUAL(d, 101.101);
	std::cout << "n:" << n << ", d:" << d << "\n";
	}

	Py_DECREF(func);
	unload_module(module);
}

BOOST_AUTO_TEST_CASE(tuple_return)
{
	PyObject * module = load_module();
	PyObject * func = PyObject_GetAttrString(module, "tuple_return");

	{
	pyresult result(PyObject_CallObject(func, NULL));
	long n;
	double d;
	result >> n >> d;

	BOOST_CHECK_EQUAL(n, 102);
	BOOST_CHECK_EQUAL(d, 102.102);
	std::cout << "n:" << n << ", d:" << d << "\n";
	}

	Py_DECREF(func);
	unload_module(module);
}

BOOST_AUTO_TEST_CASE(integer_return)
{
	PyObject * module = load_module();
	PyObject * func = PyObject_GetAttrString(module, "integer_return");

	{
	pyresult result(PyObject_CallObject(func, NULL));
	long n;
	result >> n;
	std::cout << "n:" << n << "\n";
	}

	Py_DECREF(func);
	unload_module(module);
}

BOOST_AUTO_TEST_CASE(list_return_complex)
{
	PyObject * module = load_module();
	PyObject * func = PyObject_GetAttrString(module, "list_return_complex");

	{
	pyresult result(PyObject_CallObject(func, NULL));
	long n;
	double d;
	vector<int> v;
	result >> n >> d >> v;

	BOOST_CHECK_EQUAL(n, 101);
	BOOST_CHECK_EQUAL(d, 101.101);

	int expected[3] = {1, 0, 1};
	for (int i = 0; i < v.size(); ++i)
		BOOST_CHECK_EQUAL(v[i], expected[i]);

	std::cout << "n:" << n << ", d:" << d << ", v:(";
	for (auto e : v)
		std::cout << e << ",";
	std::cout << ")\n";
	}

	Py_DECREF(func);
	unload_module(module);
}

PyObject * load_module()
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

	return pModule;
}

void unload_module(PyObject * module)
{
	Py_DECREF(module);
	Py_Finalize();
}

