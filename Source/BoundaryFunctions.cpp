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

void BoundaryObject::ApplyBoundaryCondition(PreprocessorData* preProcData, SolutionData* solutionData)
{
	int NB = preProcData->nBoundaryPoints;
	int indexStart = m_boundaryMemberIndices.front();
	int indexEnd = m_boundaryMemberIndices.back();

	for (int i = indexStart; i <= indexEnd; i++)
	{
		double g1 = m_gamma1;
		double g2 = m_gamma2;
		double g3 = m_gamma3;

		double range = preProcData->dL[i] / 4.;
		solutionData->c_next[i] = (g3 * range + g2 * solutionData->c_next[NB + i]) / (g1 * range + g2);
		solutionData->cb_next[i] = (g3 * range + g2 * solutionData->cb_next[NB + i]) / (g1 * range + g2);
	}
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