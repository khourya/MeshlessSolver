#pragma once

#include <assert.h>
// #include "Structures.h"
#include "MeshlessSolver.h"

namespace Test
{
	bool ExpectEq(double expectedSolution, double actualSolution, const char* testName);
	bool ExpectNear(double expectedSolution, double actualSolution, double tol, const char* testName);
	void TestAssert(bool bAssert, const char* testName);
}

namespace RBFUnitTests
{
	bool TestRBF();
	bool TestRBF_dx();
	bool TestRBF_dy();
}

namespace LUUnitTests
{
	bool TestLUDecomp();
	bool TestLUBacksub();
	bool TestLU4x4();
}

namespace MathUtilTests
{
	bool TestIntegration_ConstantField();
	bool TestIntegration_LinearField();
	bool TestIntegration_PentagonalPrism();
	bool TestIntegration_2DEdge();
	bool TestIntegration_IrregularPrism();
}

class Tester
{

};