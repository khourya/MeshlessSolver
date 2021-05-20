#include "MeshlessSolver.h"

// Global collocation methods
void GlobalCollocation::displayStrat()
{
	std::cout << "Implementing global collocation." << endl;
}

int GlobalCollocation::Topology(bool* checker, GeometricData* preProcData, CollocationData* topologyData)
{
	int errorFlag = 0;

	/*
	
	Do Nothing
	Global Collocation schemes do not build local topologies.
	
	*/

	return errorFlag;
}

int GlobalCollocation::Collocate(bool* checker, GeometricData* preProcData, CollocationData* topologyData, InterpolationVectors* interpolationVectors)
{
	int errorFlag = 0;
	return errorFlag;
}