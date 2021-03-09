#include "Structures.h"

BoundaryObject::BoundaryObject(int boundaryKind, int NS, double diffusionCoefficient, double boundaryValue, double vehicleVolume)
	: m_boundaryKind(static_cast<boundaryType>(boundaryKind)), m_gamma3(boundaryValue), m_volume(vehicleVolume)
{
	switch (m_boundaryKind)
	{
	case boundaryType::FirstKind:
		m_gamma1 = 1.;
		m_gamma2 = 0.;
		break;
	case boundaryType::SecondKind:
		m_gamma1 = 0.;
		m_gamma2 = -diffusionCoefficient;
		break;
	case boundaryType::ThirdKind:
		m_gamma1 = 1.;
		m_gamma2 = diffusionCoefficient;
		break;
	case boundaryType::FiniteVehicle:
		m_gamma1 = 1.;
		m_gamma2 = diffusionCoefficient;
		break;
	}

	m_jDotN_old = std::vector<double>(NS, 0.);

}

int BoundaryObject::ApplyBoundaryCondition(PreprocessorData* preProcData, SolutionData* solutionData)
{
	int errorFlag = 0;

	int NB = preProcData->nBoundaryPoints;

	for (int i : m_boundaryMemberIndices)
	{
		double g1 = m_gamma1;
		double g2 = m_gamma2;
		double g3 = m_gamma3;
		double range = preProcData->dL[i] / 4.;

		int shadowPointIndex = NB + i;

		solutionData->c_next[i] = (g3 * range + g2 * solutionData->c_next[shadowPointIndex]) / (g1 * range + g2);
		solutionData->cb_next[i] = (g3 * range + g2 * solutionData->cb_next[shadowPointIndex]) / (g1 * range + g2);
	}

	return errorFlag;
}

int BoundaryObject::UpdateDonorVolume(double dT, PreprocessorData* preProcData, SolutionData* solutionData)
{
	int errorFlag = 0;

	// We only need to update volumes for boundaries that are of the finite volume type
	if (m_boundaryKind != boundaryType::FiniteVehicle)
		return errorFlag;

	std::vector<double> jDotN_current;
	std::vector<double> x;
	std::vector<double> y;

	for (int i : m_boundaryMemberIndices)
	{
		double flux_x = solutionData->fTx_bnd[i];
		double flux_y = solutionData->fTy_bnd[i];

		double nX = preProcData->dNx[i];
		double nY = preProcData->dNy[i];

		jDotN_current.push_back(flux_x * nX + flux_y * nY);

		x.push_back(preProcData->Xc[i]);
		y.push_back(preProcData->Yc[i]);
	}


	// Account for mass of compound that has diffused into solution:
	double dMass = 0.;
	errorFlag = Integrate(dT, x, y, m_jDotN_old, jDotN_current, dMass);

	m_gamma3 = m_gamma3 - dMass / m_volume;

	m_jDotN_old = jDotN_current;

	return errorFlag;
}

void BoundaryObject::AddNode(int index)
{
	m_boundaryMemberIndices.push_back(index);
}

void BoundaryObject::AddNormalVectors(double dNx, double dNy)
{
	m_dNx.push_back(dNx);
	m_dNy.push_back(dNy);
}