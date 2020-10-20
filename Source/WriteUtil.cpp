#include <iostream>

#include "MeshlessSolver.h"

void WriteCSV(double NB, std::vector<double> col1, std::vector<double> col2)
{
	std::ofstream myFile;
	myFile.open("Data\\LCMMOutput.csv");

	size_t I = col1.size();
	size_t J = 2;
	
	for (size_t i = 0; i < I; i++)
	{
		if (i < NB)
		{
			myFile << i + 1 << "," << col1[i] << "," << col2[i] << endl;
		}
		else
		{
			myFile << i + 1 - NB << "," << col1[i] << "," << col2[i] << endl;
		}
	}
	myFile.close();
}

void WriteMatrix(std::vector<std::vector<double>> A, std::vector<const char*> headers)
{
	std::ofstream myFile;
	myFile.open("Data\\LCMMMatrix.txt");

	// Writing Headers
	size_t I = headers.size();
	for (size_t i = 0; i < I; i++)
	{
		if (i == I - 1)
			myFile << headers[i] << "\n";
		else
			myFile << headers[i] << ",";
	}

	// Writing Data
	I = A.size();
	for (size_t i = 0; i < I; i++)
	{
		size_t J = A[i].size();
		for (size_t j = 0; j < J; j++)
		{
			if (j == J - 1)
				myFile << A[i][j] << "\n";
			else
				myFile << A[i][j] << ",";
		}
	}

	myFile.close();
}

void WriteShapeFactors(std::vector<double> factors, std::vector<double> shapeFactors, std::vector<int> nInfluencePoints)
{
	std::ofstream myFile;
	myFile.open("Data\\ShapeFactors.txt");
	myFile << "Index,Factor,Shape Factor,nInfluencePoints" << endl;

	size_t I = shapeFactors.size();
	for (size_t i = 0; i < I; i++)
	{
		myFile << i + 1 << "," << factors[i] << "," << shapeFactors[i] << "," << nInfluencePoints[i] << endl;
	}
	myFile.close();
}

void WriteOutput(double step, double time, PreprocessorData* preProcData, SolutionData* solutionData)
{
	const std::string outputFile = "Data\\LCMMOutput.txt";
	std::vector<const char*> headers = { "Step", "Time [s]", "X", "Y", "Concentration", "Fx", "Fy" };
	std::ofstream myFile;

	// If first time step: clear file, write headers, write data
	if (step == 0)
	{
		// Open as new file
		myFile.open(outputFile, std::ios::out);

		// Writing Headers
		size_t I = headers.size();
		for (size_t i = 0; i < I; i++)
		{
			if (i == I - 1)
				myFile << headers[i] << "\n";
			else
				myFile << headers[i] << ",";
		}

		// Writing Data
		for (int i = 0; i < preProcData->nInternalPoints; i++)
		{
			if (i < preProcData->nBoundaryPoints)
			{
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_bnd[i] << "," << solutionData->fTx_bnd[i] << "," << solutionData->fTy_bnd[i] << endl;
			}
			else
			{
				myFile << step << "," << time << "," << i + 1 - preProcData->nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_int[i] << "," << solutionData->fTx_int[i] << "," << solutionData->fTy_int[i] << endl;
			}
		}

	}
	// Otherwise just append data to the file
	else
	{
		// Open for appending
		myFile.open(outputFile, std::ios::app);

		// Writing Data
		for (int i = 0; i < preProcData->nInternalPoints; i++)
		{
			if (i < preProcData->nBoundaryPoints)
			{
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_bnd[i] << "," << solutionData->fTx_bnd[i] << "," << solutionData->fTy_bnd[i] << endl;
			}
			else
			{
				myFile << step << "," << time << "," << i + 1 - preProcData->nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_int[i] << "," << solutionData->fTx_int[i] << "," << solutionData->fTy_int[i] << endl;
			}
		}
	}

	myFile.close();
}