#pragma once
#include <Python.h>
#include <ostream>

std::ostream & operator<<(std::ostream & out, PyObject * py);
