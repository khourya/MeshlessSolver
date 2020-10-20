#include "MeshlessSolver.h"

int Collocation(bool* checker, Options* options, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors)
{
	int errorFlag = 0;

	CollocationMethod strategy(options->topologyGenFlag);
	if (strategy.m_collocationStrategy == nullptr)
	{
		std::cout << endl << "Collocation strategy pointer is null." << endl;
		errorFlag = -4000;
		return errorFlag;
	}

	if (options->topologyGenFlag != collocationStratFlag::Global)
	{
		std::cout << "     Building local topologies.............................";
		errorFlag = strategy.m_collocationStrategy->Topology(checker, preProcData, collocationData);
		std::cout << " done!" << std::endl;
	}

	std::cout << "     Performing Collocation................................";
	errorFlag = strategy.m_collocationStrategy->Collocate(checker, preProcData, collocationData, interpolationVectors);
	std::cout << " done!" << std::endl;

	return errorFlag;
}