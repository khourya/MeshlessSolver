#include "BoundaryDef.h"

BoundaryObject::BoundaryObject(int boundaryKind, double diffusionCoefficient, double boundaryValue) : m_boundaryKind(boundaryKind)
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
}

void BoundaryObject::ApplyBoundaryCondition(SolutionData& solutionData)
{
	// do nothing atm
}