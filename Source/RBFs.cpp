#include <iostream>

#include "UnitTestUtil.h"
#include "MeshlessSolver.h"

double RBF(double x, double y, double xj, double yj, double c)
{
	double r2 = (x - xj) * (x - xj) + (y - yj) * (y - yj);
	double c2 = c * c;

	double RBF = 1. / std::sqrt((r2 / c2) + 1.);

	return RBF;
}

double RBF_dx(double x, double y, double xj, double yj, double c)
{
	double r2 = (x - xj) * (x - xj) + (y - yj) * (y - yj);
	double c2 = c * c;

	double RBF_dx = ((xj - x) / c2) / (std::pow((r2 / c2) + 1., 1.5));

	return RBF_dx;
}

double RBF_dy(double x, double y, double xj, double yj, double c)
{
	double r2 = (x - xj) * (x - xj) + (y - yj) * (y - yj);
	double c2 = c * c;

	double RBF_dy = ((yj - y) / c2) / (std::pow((r2 / c2) + 1., 1.5));

	return RBF_dy;
}

double RBF_d2(double x, double y, double xj, double yj, double c)
{
	double r2 = (x - xj) * (x - xj) + (y - yj) * (y - yj);
	double c2 = c * c;

	double RBF_d2 = (1. / c2) * ((r2 / c2) - 2.) / (std::pow((r2 / c2) + 1., 2.5));

	return RBF_d2;
}