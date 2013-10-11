#pragma once
#include <string>
#include "pyfunc.hpp"
#include "pyresult.hpp"
#include "pymodule.hpp"

//! Call python function.
//@{
template <typename ... Args>
inline pyresult call(std::string const & module, std::string const & function,
	Args const & ... args)
{
	pymodule mod(module);
	pyfunc fce = mod(function);
	return fce(args ...);
}

template <typename ... Args>
inline pyresult call(pymodule & module, std::string const & function,
	Args const & ... args)
{
	pyfunc fce = module(function);
	return fce(args ...);
}

template <typename ... Args>
inline pyresult call(pymodule && module, std::string const & function,
	Args const & ... args)
{
	pyfunc fce = module(function);
	return fce(args ...);
}

template <typename ... Args>
inline pyresult call(pyfunc & function, Args const & ... args)
{
	return function(args ...);
}

template <typename ... Args>
inline pyresult call(pyfunc && function, Args const & ... args)
{
	return function(args ...);
}
//@}
