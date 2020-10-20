#include <iostream>

#include "MeshlessSolver.h"

int SVDecomposition(std::vector<std::vector<double>> &A,
	std::vector<double> &W,
	std::vector<std::vector<double>> &V)
{
	int errorFlag = 0;

	double tiny = 1.e-40;

	double C, F, G, H, S, X, Y, Z, SCALE, ANORM;
	int L, NM;

	G = 0.;
	SCALE = 0.;
	ANORM = 0.;

	int M = static_cast<int>(A.size());
	int N = static_cast<int>(A[0].size());

	int length = M;
	if (N > M)
	{
		length = N;
	}

	std::vector<double> RV1(length, 0.);

	for (int i = 0; i < N; i++)
	{
		L = i + 1;
		RV1[i] = SCALE * G;
		G = 0.;
		S = 0.;
		SCALE = 0.;

		if (i < M)
		{
			for (int k = i; k < M; k++)
			{
				SCALE += std::abs(A[k][i]);
			}
			if (SCALE > tiny)
			{
				for (int k = i; k < M; k++)
				{
					A[k][i] = A[k][i] / SCALE;
					S += A[k][i] * A[k][i];
				}
				F = A[i][i];
				G = -1. * copysign(std::sqrt(S), F);
				H = F * G - S;
				A[i][i] = F - G;
				if (i != N - 1)
				{
					for (int j = L; j < N; j++)
					{
						S = 0.;
						for (int k = i; k < M; k++)
						{
							S += A[k][i] * A[k][j];
						}
						F = S / H;
						for (int k = i; k < M; k++)
						{
							A[k][j] = A[k][j] + F * A[k][i];
						}
					}
				}
				for (int k = i; k < M; k++)
				{
					A[k][i] = SCALE * A[k][i];
				}
			} // if (SCALE > tiny)
		} // if (i < M)

		W[i] = SCALE * G;  // line 63
		G = 0.;
		S = 0.;
		SCALE = 0.;

		if ((i < M) && (i != N - 1))  // line 67
		{
			for (int k = L; k < N; k++)
			{
				SCALE += std::abs(A[i][k]);
			}
			if (SCALE > tiny)
			{
				for (int k = L; k < N; k++)
				{
					A[i][k] = A[i][k] / SCALE;
					S += A[i][k] * A[i][k];
				}
				F = A[i][L];
				G = -1. * copysign(std::sqrt(S), F);
				H = F * G - S;
				A[i][L] = F - G;
				for (int k = L; k < N; k++)
				{
					RV1[k] = A[i][k] / H;
				}
				if (i != M - 1)  // line 83
				{
					for (int j = L; j < M; j++)
					{
						S = 0.;
						for (int k = L; k < N; k++)
						{
							S += A[j][k] * A[i][k];
						}
						for (int k = L; k < N; k++)
						{
							A[j][k] = A[j][k] + S * RV1[k];
						}
					}
				}  // if (i != M - 1)
				for (int k = L; k < N; k++)
				{
					A[i][k] = SCALE * A[i][k];
				}
			} // if (SCALE > tiny)
		} // if ((i < M) && (i != N - 1))
		ANORM = std::max(ANORM, (std::abs(W[i]) + std::abs(RV1[i])));
	} // DO I=1,N

	for (int i = N - 1; i >= 0; i--)
	{
		if (i < N - 1)
		{
			if (std::abs(G) > tiny)
			{
				for (int j = L; j < N; j++)
				{
					V[j][i] = (A[i][j] / A[i][L]) / G;
				}
				for (int j = L; j < N; j++)
				{
					S = 0.;
					for (int k = L; k < N; k++)
					{
						S += A[i][k] * V[k][j];
					}
					for (int k = L; k < N; k++)
					{
						V[k][j] += S * V[k][i];
					}
				}
			}  // if (std::abs(G) > tiny)
			for (int j = L; j < N; j++)
			{
				V[i][j] = 0.;
				V[j][i] = 0.;
			}
		}  // if (i < N - 1)
		V[i][i] = 1.;
		G = RV1[i];
		L = i;
	}

	for (int i = N - 1; i >= 0; i--)
	{
		L = i + 1;
		G = W[i];
		if (i < N - 1)
		{
			for (int j = L; j < N; j++)
			{
				A[i][j] = 0.;
			}
		}  // if (i < N - 1)
		if (std::abs(G) > tiny)
		{
			G = 1. / G;
			if (i != N - 1)
			{
				for (int j = L; j < N; j++)
				{
					S = 0.;
					for (int k = L; k < M; k++)
					{
						S += A[k][i] * A[k][j];
					}
					F = (S / A[i][i]) * G;
					for (int k = i; k < M; k++)
					{
						A[k][j] = A[k][j] + F * A[k][i];
					}
				}
			}  // if (i != N - 1)
			for (int j = i; j < M; j++)
			{
				A[j][i] = A[j][i] * G;
			}
		}  // if (std::abs(G) > tiny)
		else
		{
			for (int j = i; j < M; j++)
			{
				A[j][i] = 0.;
			}
		}
		A[i][i] += 1.;
	}

	int ITS, IN1, IN2, IN3;

	for (int k = N - 1; k > 0; k--)
	{
		IN3 = 0;
		ITS = 0;
		while ((ITS < 500) && (IN3 == 0))
		{
			ITS++;
			IN1 = 0;
			IN2 = 0;
			L = k + 1;
			while ((L >= 1) && (IN1 == 0) && (IN2 == 0))
			{
				L = L - 1;
				NM = L - 1;
				if (std::abs(RV1[L]) < tiny)
					IN1 = 1;
				if (NM >= 1)
				{
					if (std::abs(W[NM]) < tiny)
						IN2 = 1;
				}
			}  // while ((L > 1) && (IN1 == 0) && (IN2 == 0))

			if (IN1 == 0)
			{
				C = 0.;
				S = 1.;
				for (int i = L; i < k; i++)
				{
					F = S * RV1[i];
					if (std::abs(F) > tiny)
					{
						G = W[i];
						H = std::sqrt(F * F + G * G);
						W[i] = H;
						H = 1. / H;
						C = G * H;
						S = -1. * (F * H);
						for (int j = 0; j < M; j++)
						{
							Y = A[j][NM];
							Z = A[j][i];
							A[j][NM] = (Y * C) + (Z * S);
							A[j][i] = -1. * (Y * S) + (Z * C);
						}
					}  // if (std::abs(F) > tiny)
				}
			}  // if (IN1 == 0)

			Z = W[k];
			if (L == k)
			{
				if (Z < 0.)
				{
					W[k] = -Z;
					for (int j = 0; j < N; j++)
					{
						V[j][k] = -V[j][k];
					}
				} // if (Z < 0.)
				IN3 = 1;
			}  // if (L == k)

			if (IN3 == 0)
			{
				if (ITS > 500)
				{
					std::cout << "Exceeding 500 iterations." << std::endl;
				}  // if (ITS > 500)

				X = W[L];
				NM = k - 1;
				Y = W[NM];
				G = RV1[NM];
				H = RV1[k];
				F = ((Y - Z) * (Y + Z) + (G - H) * (G + H)) / (2. * H * Y);
				G = std::sqrt(F * F + 1.);
				F = ((X - Z) * (X + Z) + H * ((Y / (F + copysign(G, F))) - H)) / X;
				C = 1.;
				S = 1.;
				for (int j = L; j < NM + 1; j++)
				{
					int i = j + 1;
					G = RV1[i];
					Y = W[i];
					H = S * G;
					G = C * G;
					Z = std::sqrt(F * F + H * H);
					RV1[j] = Z;
					C = F / Z;
					S = H / Z;
					F = (X * C) + (G * S);
					G = -1. * (X * S) + (G * C);
					H = Y * S;
					Y = Y * C;
					for (int NM = 0; NM < N; NM++)
					{
						X = V[NM][j];
						Z = V[NM][i];
						V[NM][j] = (X * C) + (Z * S);
						V[NM][i] = -1. * (X * S) + (Z * C);
					}
					Z = std::sqrt(F * F + H * H);
					W[j] = Z;
					if (std::abs(Z) > tiny)
					{
						Z = 1. / Z;
						C = F * Z;
						S = H * Z;
					}  // if (std::abs(Z) > tiny)
					F = (C * G) + (S * Y);
					X = -1. * (S * G) + (C * Y);
					for (int NM = 0; NM < M; NM++)
					{
						Y = A[NM][j];
						Z = A[NM][i];
						A[NM][j] = (Y * C) + (Z * S);
						A[NM][i] = -1. * (Y * S) + (Z * C);
					}
				}
				RV1[L] = 0.;
				RV1[k] = F;
				W[k] = X;
			}  // if (IN3 == 0)
		}  // while ((ITS < 500) && (IN3 == 0))
	}


	return errorFlag;
}
