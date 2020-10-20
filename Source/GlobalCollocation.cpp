#include "MeshlessSolver.h"

// Global collocation methods
void GlobalCollocation::displayStrat()
{
	std::cout << "Implementing global collocation." << endl;
}

int GlobalCollocation::Topology(bool* checker, PreprocessorData* preProcData, CollocationData* topologyData)
{
	int errorFlag = 0;

	/*
	
	Do Nothing
	Global Collocation schemes do not build local topologies.
	
	*/

	return errorFlag;
}

int GlobalCollocation::Collocate(bool* checker, PreprocessorData* preProcData, CollocationData* topologyData, InterpolationVectors* interpolationVectors)
{
	int errorFlag = 0;
	return errorFlag;
}