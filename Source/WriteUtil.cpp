#include <iostream>

#include "MeshlessSolver.h"
#include "WriteUtil.h"

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

void WriteOutput(double step, double time, GeometricData* preProcData, SolutionData* solutionData)
{
	WriteUnbound(step, time, preProcData, solutionData);
	WriteBound(step, time, preProcData, solutionData);
}

void WriteUnbound(double step, double time, GeometricData* preProcData, SolutionData* solutionData)
{
	const std::string outputFile = "Data\\LCMMUnboundSolution.txt";
	std::vector<const char*> headers = { "Step", "Time [s]", "N", "X", "Y", "Concentration", "Fx", "Fy" };
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
		for (int i = 0; i < preProcData->m_nInternalPoints; i++)
		{
			if (i < preProcData->m_nBoundaryPoints)
			{
				// Writes as: Step | Time | Index | Xc | Yc | Concentration | Flux_x | Flux_y
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_bnd[i] << "," << solutionData->fTx_bnd[i] << "," << solutionData->fTy_bnd[i] << endl;
			}
			else
			{
				// Writes as: Step | Time | Index | Xc | Yc | Concentration | Flux_x | Flux_y
				myFile << step << "," << time << "," << i + 1 - preProcData->m_nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_int[i] << "," << solutionData->fTx_int[i] << "," << solutionData->fTy_int[i] << endl;
			}
		}

	}
	// Otherwise just append data to the file
	else
	{
		// Open for appending
		myFile.open(outputFile, std::ios::app);

		// Writing Data
		for (int i = 0; i < preProcData->m_nInternalPoints; i++)
		{
			if (i < preProcData->m_nBoundaryPoints)
			{
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_bnd[i] << "," << solutionData->fTx_bnd[i] << "," << solutionData->fTy_bnd[i] << endl;
			}
			else
			{
				myFile << step << "," << time << "," << i + 1 - preProcData->m_nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cT_int[i] << "," << solutionData->fTx_int[i] << "," << solutionData->fTy_int[i] << endl;
			}
		}
	}

	myFile.close();
}

void WriteBound(double step, double time, GeometricData* preProcData, SolutionData* solutionData)
{
	const std::string outputFile = "Data\\LCMMBoundSolution.txt";
	std::vector<const char*> headers = { "Step", "Time [s]", "N", "X", "Y", "Concentration" };
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
		for (int i = 0; i < preProcData->m_nInternalPoints; i++)
		{
			if (i < preProcData->m_nBoundaryPoints)
			{
				// Writes as: Step | Time | Index | Xc | Yc | Concentration | <No Fluxes>
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cB_bnd[i] << endl;
			}
			else
			{
				myFile << step << "," << time << "," << i + 1 - preProcData->m_nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cB_int[i] << endl;
			}
		}

	}
	// Otherwise just append data to the file
	else
	{
		// Open for appending
		myFile.open(outputFile, std::ios::app);

		// Writing Data
		for (int i = 0; i < preProcData->m_nInternalPoints; i++)
		{
			if (i < preProcData->m_nBoundaryPoints)
			{
				// Writes as: Step | Time | Index | Xc | Yc | Concentration | <No Fluxes>
				myFile << step << "," << time << "," << i + 1 << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cB_bnd[i] << endl;
			}
			else
			{
				myFile << step << "," << time << "," << i + 1 - preProcData->m_nBoundaryPoints << "," << preProcData->Xc[i] << "," << preProcData->Yc[i] << "," << solutionData->cB_int[i] << endl;
			}
		}
	}

	myFile.close();
}

int GeometricData::WriteMeshData()
{
	const std::string outputFile = "Data\\MeshData.txt";
	std::vector<const char*> headers = { "N", "X1", "Y1", "X2", "Y2", "X3", "Y3" };
	std::ofstream myFile;

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

	int index = 0;

	for (Triangle* triangle : m_triangles)
	{
		Point* pt1 = triangle->GetPoint(1);
		double x1 = pt1->GetX();
		double y1 = pt1->GetY();

		Point* pt2 = triangle->GetPoint(2);
		double x2 = pt2->GetX();
		double y2 = pt2->GetY();

		Point* pt3 = triangle->GetPoint(3);
		double x3 = pt3->GetX();
		double y3 = pt3->GetY();

		myFile << index << ","
			<< x1 << "," << y1 << ","
			<< x2 << "," << y2 << ","
			<< x3 << "," << y3 << "\n";
		
		index++;
	}

	myFile.close();
	return true;
}