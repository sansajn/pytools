#include <Python.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>

using std::vector;
using std::cout;
using std::string;

void test_vector();
void test_simple_return();
void test_complex_return();
void test_function_call();

int main(int argc, char * argv[])
{
	test_vector();
	test_simple_return();
	test_complex_return();
	test_function_call();
	return 0;
}

void test_function_call()
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

	Py_DECREF(pName);  // ???

	PyObject * pFunc = PyObject_GetAttrString(pModule, "simple_function");
	assert(pFunc && "nepodarilo sa ziskat 'simple_function()' funkciu");
	assert(PyCallable_Check(pFunc) && "pFunc nie je volatelny objekt");

	PyObject * pArgs = PyTuple_New(0);

	PyObject * pResult = PyObject_CallObject(pFunc, pArgs);
	assert(pResult && "ziaden vysledok volanej funkcie");

	Py_DECREF(pArgs);

	Py_DECREF(pResult);

	Py_XDECREF(pFunc);
	Py_DECREF(pModule);

	Py_Finalize();
}

void test_complex_return()
{
	// vráti 'hello', 101, (3, 2, 1)
	char const * module = "test";

	Py_Initialize();
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.insert(0, '')\n");

	PyObject * pName = PyUnicode_FromString(module);
	assert(pName && "nepodarila sa konverzia na unicode string");

	PyObject * pModule = PyImport_Import(pName);
	assert(pModule && "nepodaril sa import python modulu");

	Py_DECREF(pName);  // ???

	PyObject * pFunc = PyObject_GetAttrString(pModule, "complex_return");
	assert(pFunc && "nepodarilo sa ziskat 'complex_return()' funkciu");
	assert(PyCallable_Check(pFunc) && "pFunc nie je volatelny objekt");

	PyObject * pArgs = PyTuple_New(0);

	PyObject * pResult = PyObject_CallObject(pFunc, pArgs);
	assert(pResult && "ziaden vysledok volanej funkcie");

	Py_DECREF(pArgs);

	assert(PySequence_Check(pResult) && "navratova hodnota nie je sekvencia");

	int result_size = PySequence_Size(pResult);
	assert(result_size == 3 && 
		"navratova hodnota ma nespravny pocet elementou");

	string result_s;
	int result_i;
	vector<int> result_vec;

	PyObject * pResultElem = PySequence_GetItem(pResult, 0);
	// string
	assert(PyUnicode_Check(pResultElem) && "navratova hodnota nie je retazec");
	char * str = PyUnicode_AsUTF8(pResultElem);  // netreba dealokovať ?
	result_s = str;
	Py_DECREF(pResultElem);
	cout << result_s << ", ";

	pResultElem = PySequence_GetItem(pResult, 1);
	// int
	assert(PyLong_Check(pResultElem) && "navratova hodnota nie je cele-cislo");
	result_i = PyLong_AsLong(pResultElem);
	Py_DECREF(pResultElem);
	cout << result_i << ", ";

	pResultElem = PySequence_GetItem(pResult, 2);
	// tupple/sequence
	assert(PyTuple_Check(pResultElem) && "navratova hodnota nie je n-tica");
	for (int i = 0; i < PyTuple_Size(pResultElem); ++i)
	{
		PyObject * pElem = PyTuple_GetItem(pResultElem, i);
		assert(PyLong_Check(pElem) && "ocakavam cele cislo");
		result_vec.push_back(PyLong_AsLong(pElem));
		Py_DECREF(pElem);
	}
	Py_DECREF(pResultElem);

	cout << "(";
	for (int i = 0; i < result_vec.size(); ++i)
	{
		cout << result_vec[i];
		if (i < result_vec.size()-1)
			cout << ", ";
	}
	cout << ")\n";

	Py_DECREF(pResult);

	Py_XDECREF(pFunc);
	Py_DECREF(pModule);

	Py_Finalize();
}

void test_simple_return()
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

	Py_DECREF(pName);  // ???

	PyObject * pFunc = PyObject_GetAttrString(pModule, "simple_return");
	assert(pFunc && "nepodarilo sa ziskat 'simple_return()' funkciu");
	assert(PyCallable_Check(pFunc) && "pFunc nie je volatelny objekt");

	PyObject * pArgs = PyTuple_New(0);

	PyObject * pResult = PyObject_CallObject(pFunc, pArgs);
	assert(pResult && "ziaden vysledok volanej funkcie");

	Py_DECREF(pArgs);

	assert(PyLong_Check(pResult) && "navratova hodnota nie je cele cislo");

	long result = PyLong_AsLong(pResult);

	assert(result == 101 && "nespravny vysledok");

	cout << "result:" << result << "\n";

	Py_DECREF(pResult);

	Py_XDECREF(pFunc);
	Py_DECREF(pModule);

	Py_Finalize();
}

void test_vector()
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

	Py_DECREF(pName);  // ???

	PyObject * pFunc = PyObject_GetAttrString(pModule, "print_vector");
	assert(pFunc && "nepodarilo sa ziskat 'test_vector()' funkciu");
	assert(PyCallable_Check(pFunc) && "pFunc nie je volatelny objekt");

	vector<int> data{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};  // data

	PyObject * pArgs = PyTuple_New(1);
	PyObject * pVector = PyTuple_New(data.size());

	for (int i = 0; i < data.size(); ++i)
	{
		int & elem = data[i];
		PyObject * pElem = PyLong_FromLong(elem);
		assert(pElem && "nepodarilo sa vytvorit py-objekt");
		PyTuple_SetItem(pVector, i, pElem);
	}

	PyTuple_SetItem(pArgs, 0, pVector);

	PyObject * pResult = PyObject_CallObject(pFunc, pArgs);
	assert(pResult && "ziaden vysledok volanej funkcie");

	Py_DECREF(pVector);
	Py_DECREF(pArgs);

	Py_XDECREF(pFunc);
	Py_DECREF(pModule);

	Py_Finalize();
}

