#pragma once

#include "Structures.h"
#include "Triangulate.h"

// Logging Output Data
void WriteOutput(double step, double time, GeometricData* preProcData, SolutionData* solutionData);
void WriteUnbound(double step, double time, GeometricData* preProcData, SolutionData* solutionData);
void WriteBound(double step, double time, GeometricData* preProcData, SolutionData* solutionData);
void WriteCSV(double NB, std::vector<double> col1, std::vector<double> col2);
void WriteMatrix(std::vector<std::vector<double>> A, std::vector<const char*> headers);
void WriteShapeFactors(std::vector<double> factors, std::vector<double> shapeFactors, std::vector<int> nInfluencePoints);

void LogErrors(int errorFlag);