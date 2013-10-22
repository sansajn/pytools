#pragma once
#include <Python.h>
#include <string>
#include "pyfunc.hpp"

//! Implementuje python modul.
class pymodule
{
public:
	pymodule(std::string const & module);
	~pymodule();

	pymodule(pymodule const &) = delete;
	pymodule & operator=(pymodule const &) = delete;

	pyfunc function(std::string const & name);

	//!< Shortcut for python function getter.
	pyfunc operator()(std::string const & name) {return function(name);}

private:
	PyObject * _module;
	static int module_counter;  //!< number of modules in program
};
