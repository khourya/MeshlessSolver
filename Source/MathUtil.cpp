#include "MeshlessSolver.h"

int Integrate(double dt, std::vector<double> x, std::vector<double> y, std::vector<double> s0, std::vector<double> s1, double & dResult)
{
	int errorFlag = 0;

	// Catch any dimensioning errors
	if (x.size() != y.size())
	{
		errorFlag = -5000;
		return errorFlag;
	}

	if (s0.size() != s1.size())
	{
		errorFlag = -5001;
		return errorFlag;
	}

	if (s0.size() != x.size())
	{
		errorFlag = -5002;
		return errorFlag;
	}

	double volumeIntegral = 0.;

	size_t N = s0.size();
	for (size_t n = 0; n < N - 1; n++)
	{
		double dX = x[n + 1] - x[n];
		double dY = x[n + 1] - x[n];
		double dL = std::sqrt(dX * dX + dY * dY);

		double v1 = s0[n];
		double v2 = s0[n + 1];
		double v3 = s1[n];
		double v4 = s1[n + 1];

		volumeIntegral += VolumeTruncatedRightSquarePrism(dL, dt, v1, v2, v3, v4);
	}

	dResult = volumeIntegral;

	return errorFlag;
}

double VolumeTruncatedRightTriangularPrism(double dx, double dy, double s1, double s2, double s3, double s4)
{
	double baseArea = dx * dy;
	double averageHeight = (s1 + s2 + s3 + s4) / 4;

	double volume = baseArea * averageHeight;

	return volume;
}

double VolumeTruncatedRightSquarePrism(double dx, double dy, double s1, double s2, double s3, double s4)
{
	double baseArea = dx * dy;
	double averageHeight = (s1 + s2 + s3 + s4) / 4;

	double volume = baseArea * averageHeight;

	return volume;
}

std::vector<double> CrossProduct(std::vector<double> a, std::vector<double> b)
{
	double v1 = a[1] * b[2] - a[2] * b[1];
	double v2 = -1 * (a[0] * b[2] - a[2] * b[0]);
	double v3 = a[0] * b[1] - a[1] * b[0];
	
	std::vector<double> v = { v1, v2, v3 };
	
	return v;
}