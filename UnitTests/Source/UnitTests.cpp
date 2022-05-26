// UnitTests.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "UnitTests.h"

// TODO: This is an example of a library function
void fnUnitTests()
{
	std::vector<double> v1 = { 0.0, 0.0, 0.0 };
	std::vector<double> v2 = { 0.0, 0.0, 0.0 };

	if (assertEquals(v1, v2))
		std::cout << "I called the library function!\n";

}
