#include <iostream>
#include "dict.hpp"
#include "dump.hpp"

using std::cout;


int main(int argc, char * argv[])
{
	Py_Initialize();

	{
	py::dict dict;
	int a = 0, b = 1, c = 2;
	dict << py::make_keyval(10, a) << py::make_keyval(11, b) << py::make_keyval(12, c);
	cout << dict.native() << "\n";
	}

	{
	py::dict dict2;
	dict2 << py::make_keyval(10, 0);
	cout << "dict2:" << dict2.native() << "\n";
	}

	Py_Finalize();
	return 0;
}
