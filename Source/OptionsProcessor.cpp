/***********************************************************************************************************************
**
**
**      Some government copyright stuff
**
**
***********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------
/// \file OptionsProcesser.cpp
///
/// Contains the functions for reading data from the Meshless Options File, LCMMOptions. This file contins the data to 
/// specify the parmeters/strategies for running the Meshless Code. Also contains the "debugger" functions, which ensure
/// that our read options are correct
//----------------------------------------------------------------------------------------------------------------------

#include "MeshlessSolver.h"

int ReadOptions(bool* checker, Options* options)
{
	int ierr = 0;

	std::string line;
	std::ifstream inputFile;
	inputFile.open("Data\\LCMMOptions.inp");

	if (!inputFile)
	{
		std::cout << "Unable to open 'options' file." << std::endl;
		ierr = -1000;
	}

	int tempint = 0;
	std::getline(inputFile, line);
	inputFile >> tempint;
	nodeStratFlag nodeGenFlag = static_cast<nodeStratFlag>(tempint);
	options->internalNodeGenFlag = nodeGenFlag;
	
	std::getline(inputFile, line);
	std::getline(inputFile, line);
	std::getline(inputFile, line);
	inputFile >> tempint;
	collocationStratFlag topologyFlag = static_cast<collocationStratFlag>(tempint);
	options->topologyGenFlag = topologyFlag;

	if (*checker)
		OptionsChecker(&nodeGenFlag, &topologyFlag);

	inputFile.close();

	return ierr;
}

void OptionsChecker(nodeStratFlag* nodeGenFlag, collocationStratFlag* topologyFlag)
{
	// Validating options:
	std::cout << "  Validating Options:" << std::endl;

	// Node Generation
	if (*nodeGenFlag == nodeStratFlag::Standard)
	{
		std::cout << "\tImplementing standard method for internal node generation." << std::endl;
	}
	else if (*nodeGenFlag == nodeStratFlag::Random)
	{
		std::cout << "\tImplementing random internal node generation." << std::endl;
	}
	else if (*nodeGenFlag == nodeStratFlag::Ravel)
	{
		std::cout << "\tImplementing Ravel method for internal node generation." << std::endl;
	}
	else if (*nodeGenFlag == nodeStratFlag::Jure)
	{
		std::cout << "\tImplementing Jure's method for internal node generation." << std::endl;
	}

	// Topology Generation/Collocaiton
	if (*topologyFlag == collocationStratFlag::Global)
	{
		std::cout << "\tImplementing global collocation." << std::endl;
	}
	else if (*topologyFlag == collocationStratFlag::Local)
	{
		std::cout << "\tImplementing standard local collocation method." << std::endl;
	}
	else if (*topologyFlag == collocationStratFlag::LocalwPoly)
	{
		std::cout << "\tImplementing local collocation with polynomial augmentation." << std::endl;
	}
}
