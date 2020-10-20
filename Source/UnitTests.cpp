#include "UnitTestUtil.h"

bool RunUnitTests()
{
	bool passedTest = true;

	passedTest = RBFUnitTests::TestRBF();
	if (!passedTest)
		return passedTest;

	passedTest = LUUnitTests::TestLUDecomp();
	passedTest = LUUnitTests::TestLUBacksub();
	passedTest = LUUnitTests::TestLU4x4();
	if (!passedTest)
		return passedTest;

	return passedTest;
}

namespace RBFUnitTests
{
	bool TestRBF()
	{
		bool passed = true;
		// Case 1: Arbitrary pole and influence point

		passed = Test::ExpectEq(1., 1., "RBF Case 1");

		// Case 2: Pole at zero, arbitrary influence point
		passed = Test::ExpectEq(1., 1., "RBF Case 1");

		// Case 3: Pole and influence point coincide
		passed = Test::ExpectEq(1., 1., "RBF Case 1");

		// Case 4: 

		return passed;
	}


	bool TestRBF_dx()
	{
		bool passed = true;
		passed = Test::ExpectEq(1., 1., "RBF Case 1");
		return passed;
	}

	bool TestRBF_dy()
	{
		bool passed = true;
		passed = Test::ExpectEq(1., 2., "RBF Case 3");
		return passed;
	}

}

namespace SVDUnitTests
{
	bool TestSVD()
	{
		std::vector<std::vector<double>> A;
		std::vector<std::vector<double>> V;
		std::vector<double> W;

		int I = 3;
		int J = 3;

		A.push_back({ 0.8, 0.3, 0.4 });
		A.push_back({ 0.2, 0.7, 0.5 });
		A.push_back({ 0.3, 0.2, 0.2 });

		V.push_back({ 0., 0., 0. });
		V.push_back({ 0., 0., 0. });
		V.push_back({ 0., 0., 0. });

		W.push_back(0.);
		W.push_back(0.);
		W.push_back(0.);

		LogMat("A", A);
		LogMat("V", V);
		LogMat("W", W);

		int errorFlag = SVDecomposition(A, W, V);

		LogMat("V", V);
		LogMat("W", W);

		if (errorFlag > 0)
			return false;

		return true;
	}
	
}

namespace LUUnitTests
{
	bool TestLUDecomp()
	{
		bool passed = true;

		std::vector<std::vector<double>> A;
		LogMat("A", A);

		std::vector<int> index (3, 0);
		double D = 0.;

		A.push_back({ 0.8, 0.3, 0.4 });
		A.push_back({ 0.2, 0.5, 0.5 });
		A.push_back({ 0.1, 0.3, 0.6 });

		int errorFlag = LUDecomposition(A, 3, index, D);
		LogMat("A", A);
		LogMat("I", index);

		// passed = Test::ExpectEq()

		return passed;
	}

	bool TestLUBacksub()
	{
		bool passed = true;

		std::vector<std::vector<double>> A;
		std::vector<std::vector<double>> Ai;
		LogMat("A", A);

		std::vector<int> index(3, 0);
		std::vector<double> BV(3, 0.);
		double D = 0.;

		A.push_back({ 0.8, 0.3, 0.4 });
		A.push_back({ 0.2, 0.5, 0.5 });
		A.push_back({ 0.1, 0.3, 0.6 });

		Ai.push_back({ 0., 0., 0. });
		Ai.push_back({ 0., 0., 0. });
		Ai.push_back({ 0., 0., 0. });

		int errorFlag = LUDecomposition(A, 3, index, D);
		LogMat("A", A);

		for (int jj = 0; jj < 3; jj++)
		{
			for (int ii = 0; ii < 3; ii++)
			{
				BV[ii] = 0.;
			}
			BV[jj] = 1.;
			LUBackSubstitution(A, 3, index, BV);
			for (int ii = 0; ii < 3; ii++)
			{
				Ai[ii][jj] = BV[ii];
			}
		}

		LogMat("Ai", Ai);

		return passed;
	}

	bool TestLU4x4()
	{
		std::cout << "LU 4x4 Test" << std::endl;
		bool passed = true;
		int errorFlag = 0;

		std::vector<std::vector<double>> A;
		std::vector<std::vector<double>> Ai;
		std::vector<double> BV(4, 0.);
		std::vector<int> index(4, 0);
		double D = 0.;

		A.push_back({ 4., 2., 5., 9. });
		A.push_back({ 6., 3., 7., 1. });
		A.push_back({ 2., 7., 4., 8. });
		A.push_back({ 1., 3., 1., 2. });

		Ai.push_back({ 0., 0., 0., 0. });
		Ai.push_back({ 0., 0., 0., 0. });
		Ai.push_back({ 0., 0., 0., 0. });
		Ai.push_back({ 0., 0., 0., 0. });

		int dim1 = static_cast<int>(A.size());

		// LU Decomposition
		errorFlag = LUDecomposition(A, dim1, index, D);
		std::cout << "LU Decomposition of 4x4" << std::endl;
		LogMat("A", A);

		// LU Backsubstitution for Inversion
		// errorFlag = LUBackSubstitution(A, dim1, index, BV);
		for (int jj = 0; jj < dim1; jj++)
		{
			for (int ii = 0; ii < dim1; ii++)
			{
				BV[ii] = 0.;
			}
			BV[jj] = 1.;
			LUBackSubstitution(A, dim1, index, BV);
			for (int ii = 0; ii < dim1; ii++)
			{
				Ai[ii][jj] = BV[ii];
			}
		}

		std::cout << "Inverse of 4x4" << std::endl;
		LogMat("Ai", Ai);

		return passed;
	}
}