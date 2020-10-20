#include "UnitTestUtil.h"

namespace Test
{
	double tinyTol = 1.e-40;
}

bool Test::ExpectEq(double expectedSolution, double actualSolution, const char* testName)
{
	bool isEqual = true;
	double diff = std::abs(expectedSolution - actualSolution);

	if (diff > Test::tinyTol)
		isEqual = false;

	Test::TestAssert(isEqual, testName);

	return isEqual;
}

bool Test::ExpectNear(double expectedSolution, double actualSolution, const char* testName)
{
	bool isNear = true;
	double diff = std::abs(expectedSolution - actualSolution);

	if (diff > Test::tinyTol)
		isNear = false;

	Test::TestAssert(isNear, testName);
	
	return isNear;
}

void Test::TestAssert(bool bAssert, const char* testName)
{
	if (!bAssert)
	{
		std::cout << "We failed: " << testName << std::endl;
	}
}