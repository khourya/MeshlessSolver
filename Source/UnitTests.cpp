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

	passedTest = MathUtilTests::TestIntegration_ConstantField();
	passedTest = MathUtilTests::TestIntegration_LinearField();
	passedTest = MathUtilTests::TestIntegration_PentagonalPrism();
	passedTest = MathUtilTests::TestIntegration_2DEdge();
	passedTest = MathUtilTests::TestIntegration_IrregularPrism();

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

namespace MathUtilTests
{
	double tol = 1.; // percent

	bool TestIntegration_ConstantField()
	{
		bool passed = true;
		int errorFlag = 0;

		double Length = 10.;
		double dT = 0.1;
		int vecSize = 101;

		// Constant value over time and space
		std::vector<double> F(vecSize, 2.);
		std::vector<double> x(vecSize, 0.);
		std::vector<double> y(vecSize, 0.);

		for (int i = 0; i < vecSize; i++)
		{
			x[i] = i * (Length / (vecSize - 1));
		}

		double actualSolution_1 = 0.;
		double actualSolution_2 = 0.;
		double expectedSolution = 2.;

		errorFlag = Integrate(dT, x, y, F, F, actualSolution_2);
		passed = Test::ExpectNear(expectedSolution, actualSolution_1, tol, "Integration: Linearly Increasing: Prism");
		passed = Test::ExpectNear(expectedSolution, actualSolution_2, tol, "Integration: Linearly Increasing: Trap");

		if (errorFlag != 0)
			passed = false;

		return passed;
	}

	bool TestIntegration_LinearField()
	{
		bool passed = true;
		int errorFlag = 0;

		double Length = 10.;
		double maxVal = 15.;
		double minVal = 3.;
		double dT = 0.5;
		int vecSize = 101;

		// Constant value over time and space
		std::vector<double> F(vecSize, 0.);
		std::vector<double> x(vecSize, 0.);
		std::vector<double> y(vecSize, 0.);

		for (int i = 0; i < vecSize; i++)
		{
			x[i] = i * (Length / (vecSize - 1));
			F[i] = i * ((maxVal - minVal) / (vecSize - 1)) + minVal;
		}

		double actualSolution_1 = 0.;
		double actualSolution_2 = 0.;
		double expectedSolution = 45.;

		errorFlag = Integrate(dT, x, y, F, F, actualSolution_2);
		passed = Test::ExpectNear(expectedSolution, actualSolution_1, tol, "Integration: Linearly Increasing then Decreasing: Prism");
		passed = Test::ExpectNear(expectedSolution, actualSolution_2, tol, "Integration: Linearly Increasing then Decreasing: Trap");

		if (errorFlag != 0)
			passed = false;

		return passed;
	}

	bool TestIntegration_PentagonalPrism()
	{
		bool passed = true;
		int errorFlag = 0;

		// x-dim
		double Length = 10.;
		int vecSize = 101;
		double dT = 0.5;

		double maxVal = 15.;
		double minVal = 5.;

		// Constant value over time and space
		std::vector<double> x(vecSize, 0.);
		std::vector<double> y(vecSize, 0.);
		std::vector<double> F(vecSize, 0.);

		for (int i = 0; i < vecSize; i++)
		{
			x[i] = i * (Length / (vecSize - 1));
		}

		int mp = (vecSize - 1) / 2;
		for (int i = 0; i <= mp; i++)
		{	
			double value = (mp - i) * ((maxVal - minVal) / mp) + minVal;

			F[mp + i] = value;
			F[mp - i] = value;
		}

		double actualSolution_1 = 0.;
		double actualSolution_2 = 0.;
		double expectedSolution = 50.;

		errorFlag = Integrate(dT, x, y, F, F, actualSolution_2);
		passed = Test::ExpectNear(expectedSolution, actualSolution_1, tol, "Integration: Pentagonal Prism: Prism");
		passed = Test::ExpectNear(expectedSolution, actualSolution_2, tol, "Integration: Pentagonal Prism: Trap");

		if (errorFlag != 0)
			passed = false;

		return passed;
	}

	bool TestIntegration_2DEdge()
	{
		bool passed = true;
		int errorFlag = 0;

		// x-dim
		double length_X = 3.;
		double length_Y = 4.;
		int vecSize = 101;
		double dT = 5.;

		double maxVal = 15.;
		double minVal = 5.;

		// Constant value over time and space
		std::vector<double> x(vecSize, 0.);
		std::vector<double> y(vecSize, 0.);
		std::vector<double> F(vecSize, 10.);

		for (int i = 0; i < vecSize; i++)
		{
			x[i] = i * (length_X / (vecSize - 1));
			y[i] = i * (length_Y / (vecSize - 1));
		}

		double actualSolution_1 = 0.;
		double actualSolution_2 = 0.;
		double expectedSolution = 250.;

		errorFlag = Integrate(dT, x, y, F, F, actualSolution_2);
		passed = Test::ExpectNear(expectedSolution, actualSolution_1, tol, "Integration: 2D Geometrical Edge: Prism");
		passed = Test::ExpectNear(expectedSolution, actualSolution_2, tol, "Integration: 2D Geometrical Edge: Trap");

		if (errorFlag != 0)
			passed = false;

		return passed;
	}

	bool TestIntegration_IrregularPrism()
	{
		bool passed = true;
		int errorFlag = 0;

		double length = 30.;
		int vecSize = 101;
		double dT = 20.;

		double max_0 = 30.;
		double min_0 = 15.;
		double max_1 = 40.;
		double min_1 = 20.;

		// Constant value over time and space
		std::vector<double> x(vecSize, 0.);
		std::vector<double> y(vecSize, 0.);
		std::vector<double> F0(vecSize, 0.);
		std::vector<double> F1(vecSize, 0.);

		for (int i = 0; i < vecSize; i++)
		{
			x[i] = i * (length / (vecSize - 1));
			F0[i] = i * (max_0 - min_0) / (vecSize - 1) + min_0;
			F1[i] = i * (max_1 - min_1) / (vecSize - 1) + min_1;
		}

		double actualSolution_1 = 0.;
		double actualSolution_2 = 0.;
		double expectedSolution = 15500.;

		errorFlag = Integrate(dT, x, y, F0, F1, actualSolution_2);
		passed = Test::ExpectNear(expectedSolution, actualSolution_1, tol, "Integration: Irregular Prism: Prism");
		passed = Test::ExpectNear(expectedSolution, actualSolution_2, tol, "Integration: Irregular Prism: Trap");


		if (errorFlag != 0)
			passed = false;

		return passed;
	}
}