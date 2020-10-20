#include <iostream>

#include "MeshlessSolver.h"

int Triangulate(bool* checker, PreprocessorData* preProcData)
{
	int errorFlag = 0;
	
	if (*checker)
	{
		errorFlag = 0;
	}

	if (preProcData->nBoundaryPoints > NBMax)
	{
		errorFlag = -1;
	}

	return errorFlag;

}