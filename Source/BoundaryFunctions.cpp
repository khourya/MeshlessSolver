#include "BoundaryDef.h"

BoundaryObject::BoundaryObject(int boundaryKind, double diffusionCoefficient, double boundaryValue, double vehicleVolume) : m_boundaryKind(boundaryKind)
{
	switch (boundaryKind)
	{
	case 1:
		m_gamma1 = 1.;
		m_gamma2 = 0.;
		break;
	case 2:
		m_gamma1 = 0.;
		m_gamma2 = -diffusionCoefficient;
		break;
	case 3:
		m_gamma1 = 1.;
		m_gamma2 = diffusionCoefficient;
		break;
	}

	m_gamma3 = boundaryValue;
	m_volume = vehicleVolume;
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

int BoundaryObject::UpdateDonorVolume(PreprocessorData* preProcData, SolutionData* solutionData)
{
	int errorFlag = 0;
	std::vector<double> flux_x1;
	std::vector<double> flux_y1;
	// copy(vect1.begin(), vect1.end(), back_inserter(vect2));

	std::vector<double>::const_iterator first = solutionData->fTx_bnd.begin() + m_boundaryMemberIndices.front();
	std::vector<double>::const_iterator last = solutionData->fTx_bnd.begin() + m_boundaryMemberIndices.back() + 1;
	std::vector<double> flux_x2(first, last);

	first = solutionData->fTx_bnd.begin() + m_boundaryMemberIndices.front();
	last = solutionData->fTx_bnd.begin() + m_boundaryMemberIndices.back() + 1;
	std::vector<double> flux_y2(first, last);


	for (int i : m_boundaryMemberIndices)
	{
		double flux_x = solutionData->fTx_bnd[i];
		double flux_y = solutionData->fTy_bnd[i];

		flux_x1.push_back(flux_x);
		flux_y1.push_back(flux_y);

		double nX = preProcData->dNx[i];
		double nY = preProcData->dNy[i];

		flux_x = nX * flux_x;
		flux_y = nY * flux_y;
	}

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