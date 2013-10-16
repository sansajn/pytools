#pragma once
#include <Python.h>

namespace py {

template <typename T>
inline PyObject * pytonize(T const & val);

template <>
inline PyObject * pytonize<long>(long const & val)
{
	return PyLong_FromLong(val);
}

template <>
inline PyObject * pytonize<int>(int const & val)
{
	return pytonize<long>(val);
}

template <>
inline PyObject * pytonize<double>(double const & val)
{
	return PyFloat_FromDouble(val);
}

template <>
inline PyObject * pytonize<float>(float const & val)
{
	return pytonize<double>(val);
}


// mechanizmus pre list/tuple konverziu
void as_list();
void as_tuple();


// key-value map hepler (urcena k citaniu)
template <typename K, typename V>
struct keyval
{
	K const & key;
	V const & value;
	keyval(K const & k, V const & v) : key(k), value(v) {}
};

template <typename K, typename V>
keyval<K, V> make_keyval(K const & k, V const & v)
{
	return keyval<K, V>(k, v);
}


class dict
{
public:
	dict() {_obj = PyDict_New();}

	~dict()
	{
		if (_obj)
			Py_DECREF(_obj);
	}

	dict(dict const &) = delete;
	dict & operator=(dict const &) = delete;

	template <typename K, typename V>
	dict & operator<<(keyval<K, V> const & rhs)
	{
		PyObject * key = pytonize(rhs.key);
		PyObject * val = pytonize(rhs.value);
		PyDict_SetItem(_obj, key, val);
		return *this;
	}

	PyObject * native() const {return _obj;}

private:
	PyObject * _obj;
};

} // py
