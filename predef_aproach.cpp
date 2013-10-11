/* Overenie preddefinovaneho konceptu, do vektoru prečítam tuple,
alebo list, ... .
Umoznuje citanie vysledku zo sekvencie aj jednoducheho typu. */
#include <Python.h>
#include <vector>
#include <memory>
#include <iostream>

// elementary types support (common for tuple/list/...)
template <typename R>
R get(PyObject * o);

template <>
long get<long>(PyObject * o)
{
	assert(PyLong_Check(o) && "python long object expected");
	return PyLong_AsLong(o);
}

template <>
double get<double>(PyObject * o)
{
	assert(PyFloat_Check(o) && "python float object expected");
	return PyFloat_AsDouble(o);
}

template <typename R>
R sequence_at(PyObject * o, size_t pos)
{
	assert(PySequence_Check(o) && "object doesn't implement sequence protocol");
	assert(pos < PySequence_Size(o) && "position out of range");
	PyObject * item = PySequence_GetItem(o, pos);
	return get<R>(item);
}

struct result_type 
{
public:
	result_type(PyObject * o) 
		: _pos(0), _result(o) 
	{}

	template <typename T>
	result_type & operator>>(T & val) 
	{
		if (!PySequence_Check(_result))
		{
			val = get<T>(_result);
			assert(_pos++ == 0 && "viacnasobne citanie jednoduchej hodnoty");
		}
		else
			val = sequence_at<T>(_result, _pos++);

		return *this;
	}

	template <typename T>
	result_type & operator>>(std::vector<T> & val) 
	{
		if (!PySequence_Check(_result))
		{
			val.push_back(get<T>(_result));
			assert(_pos++ > 0 && "viacnasobne citanie jednoduchej hodnoty");
		}
		else
			for (size_t i = 0; i < PySequence_Size(_result); ++i)
				val.push_back(sequence_at<T>(_result, i));

		return *this;
	}

private:
	size_t _pos;
	PyObject * _result;
};

//! \task: implementuj oper>> pre list mimo classu


class func
{
public:
	func(PyObject * f) : _f(f) {}
	~func() {Py_DECREF(_f);}

	result_type operator()() 
	{
		assert(PyCallable_Check(_f) && "python callable object expected");
		return result_type(PyObject_CallObject(_f, NULL));
	}

private:
	PyObject * _f;
};


void test_list_return(PyObject * module)
{
	func f(PyObject_GetAttrString(module, "list_return"));
	long n;
	double d;
	f() >> n >> d;
	std::cout << "n:" << n << ", d:" << d << "\n";
}

void test_tuple_return(PyObject * module)
{
	func f(PyObject_GetAttrString(module, "tuple_return"));
	long n;
	double d;
	f() >> n >> d;
	std::cout << "n:" << n << ", d:" << d << "\n";
}

void test_integer_return(PyObject * module)
{
	func f(PyObject_GetAttrString(module, "integer_return"));
	long n;
	f() >> n;
	std::cout << "n:" << n << "\n";
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


int main(int rgc, char * argv[])
{
	PyObject * module = load_module();
	
	test_list_return(module);
	test_tuple_return(module);
	test_integer_return(module);

	unload_module(module);
	return 0;
}

