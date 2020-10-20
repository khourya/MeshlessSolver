#pragma once

#include "Structures.h"

class BoundaryCondition
{
public:
	BoundaryCondition(int boundaryType) : m_boundaryKind(boundaryType)
	{
	}

	void ApplyBoundaryCondition();

private:
	int m_boundaryKind;
};