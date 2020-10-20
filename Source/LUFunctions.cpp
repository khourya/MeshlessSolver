#include "MeshlessSolver.h"

int LUDecomposition(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, double &D)
{
	int errorFlag = 0;

	size_t N = static_cast<size_t> (nInfluencePoints);
	D = 1.;

	double AAMAX, dum;
	double epsilon = 1.e-6;
	std::vector<double> VV(N, 0.);

	for (int i = 0; i < N; i++)
	{
		AAMAX = 0.;
		for (int j = 0; j < N; j++)
		{
			if (std::abs(A[i][j]) > AAMAX)
				AAMAX = std::abs(A[i][j]);
		}
		if (AAMAX == 0.)
		{
			errorFlag = -100; // SINGULAR MATRIX
		}
		VV[i] = 1. / AAMAX;
	}

	for (int j = 0; j < N; j++)
	{
		int iMax = 0;

		if (j > 0)
		{
			for (int i = 0; i < j; i++)
			{
				double sum = A[i][j];
				if (i > 0)
				{
					for (int k = 0; k < i; k++)
					{
						sum -= A[i][k] * A[k][j];

					}
					A[i][j] = sum;
				}
			}
		}

		AAMAX = 0;

		for (int i = j; i < N; i++)
		{
			double sum = A[i][j];
			if (j > 0)
			{
				for (int k = 0; k < j; k++)
				{
					sum -= A[i][k] * A[k][j];
				}
				A[i][j] = sum;
			}
			dum = VV[i] * std::abs(sum);
			if (dum >= AAMAX)
			{
				iMax = i;
				AAMAX = dum;
			}
		}

		if (j != iMax)
		{
			for (int k = 0; k < N; k++)
			{
				dum = A[iMax][k];
				A[iMax][k] = A[j][k];
				A[j][k] = dum;
			}
			D = -D;
			VV[iMax] = VV[j];
		}

		index[j] = iMax;

		if (j != N-1)
		{
			if (A[j][j] == 0.)
				A[j][j] = epsilon;

			dum = 1. / A[j][j];
			for (int i = j + 1; i < N; i++)
			{
				A[i][j] *= dum;
			}
		}
	}

	if (A[N-1][N-1] == 0.)
		errorFlag = -101; // SINGULAR MATRIX

	return errorFlag;
}

int LUBackSubstitution(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, std::vector<double> &BV)
{
	int errorFlag = 0;

	int II = -1;
	int N = nInfluencePoints;
	double sum = 0.;

	for (int i = 0; i < N; i++)
	{
		int LL = index[i];
		sum = BV[LL];
		BV[LL] = BV[i];
		if (II != -1)
		{
			for (int j = II; j < i; j++)
			{
				sum -= A[i][j] * BV[j];
			}
		}
		else if (sum != 0.)
		{
			II = i;
		}
		BV[i] = sum;
	}

	for (int i = N - 1; i >= 0; i--)
	{
		sum = BV[i];
		if (i < N - 1)
		{
			for (int j = i + 1; j < N; j++)
			{
				sum = sum - A[i][j] * BV[j];
			}
		}
		BV[i] = sum / A[i][i];
	}

	return errorFlag;
}