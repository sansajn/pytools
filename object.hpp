#pragma once
#include <Python.h>


namespace py {

template <typename T>
inline PyObject * pythonize(T const & val);

template <>
inline PyObject * pythonize<long>(long const & val)
{
	return PyLong_FromLong(val);
}

template <>
inline PyObject * pythonize<int>(int const & val)
{
	return pythonize<long>(val);
}

template <>
inline PyObject * pythonize<double>(double const & val)
{
	return PyFloat_FromDouble(val);
}

template <>
inline PyObject * pythonize<float>(float const & val)
{
	return pythonize<double>(val);
}

// mechanizmus pre list/tuple pythonizaciu
namespace detail {

template <typename T>
struct list_type_holder
{
	T const & holdee;
	list_type_holder(T const & rhs) : holdee(rhs) {}
};

template <typename T>
struct tuple_type_holder
{
	T const & holdee;
	tuple_type_holder(T const & rhs) : holdee(rhs) {}
};

};  // detail

template <typename T>
inline detail::list_type_holder<T> as_list(T const & t)
{
	return detail::list_type_holder<T>(t);
}

template <typename T>
inline detail::tuple_type_holder<T> as_tuple(T const & t)
{
	return detail::tuple_type_holder<T>(t);
}

/*! \note pre objekt c typu C musi platit, c[n] a C::value_type */
template <typename C>
inline PyObject * pythonize(detail::tuple_type_holder<C> val)
{
	PyObject * o = PyTuple_New(val.holdee.size());
	for (size_t i = 0; i < val.holdee.size(); ++i)
		PyTuple_SetItem(o, i, pythonize<typename C::value_type>(val.holdee[i]));
	return o;
}

/*! \note pre objekt c typu C musi platit, C::const_iterator a C::value_type */
template <typename C>
inline PyObject * pythonize(detail::list_type_holder<C> val)
{
	PyObject * o = PyList_New(val.holdee.size());
	size_t i = 0;
	for (typename C::const_iterator it = begin(val.holdee); it != end(val.holdee); ++it)
		PyList_SetItem(o, i++, pythonize<typename C::value_type>(*it));
	return o;
}

};  // py
