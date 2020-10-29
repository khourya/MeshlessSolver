#include "MeshlessSolver.h"

int Integrate(double dt, std::vector<double> x, std::vector<double> y, std::vector<double> f0, std::vector<double> f1, double& dMass)
{
	int errorFlag = 0;

	if (f0.size() != f1.size())
	{
		errorFlag = -5001;
		return errorFlag;
	}

	int N = f0.size();
	for (int n = 1; n < N; n++)
	{
		double dX = x[n] - x[n - 1];
		double dY = y[n] - y[n - 1];
		double dL = std::sqrt(dX * dX + dY * dY);
	}


	return errorFlag;
}