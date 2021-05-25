#pragma once

#include "Structures.h"
#include "CollocationStrategy.h"
#include "NodeGenStrategy.h"
#include "PhysicalConstants.h"
#include "Parameters.h"
// #include "Triangulate.h"

// Commonly used character arrays
const char tab[2] = "\t";
const char endl[2] = "\n";

// Unit Tests
bool RunUnitTests();

// Reading Inputs
void InputChecker(InputData* inputData);
int ReadInputs(bool* checker, InputData* inputData, GeometricData* preProcData);

// Reading Options
void OptionsChecker(nodeStratFlag* nodeGenFlag, collocationStratFlag* collocationFlag);
int  ReadOptions(bool* checker, Options* options);

// Preprocessor Functions
int  PreprocessDomain(bool* checker, Options* options, InputData* inputData, GeometricData* preProcData, SolutionData* solution);
void SetInitialCondition(GeometricData* geometricData, SolutionData* solution);
bool isInsideDomain(double xc, double yc, GeometricData* preProcData);
bool isOnEdge(double xi, double yi, GeometricData* preProcData);
void LogBoundaryPoints(int i, int j, int nBoundaryPoints);
void LogDomainExtents(double xmin, double xmax, double ymin, double ymax);

// Triangulation Procedure for Visualization
int Triangulate(bool* checker, GeometricData* geometricData);

// RBF and Polynomial Functions
double RBF(double x, double y, double xj, double yj, double c);
double RBF_dx(double x, double y, double xj, double yj, double c);
double RBF_dy(double x, double y, double xj, double yj, double c);
double RBF_d2(double x, double y, double xj, double yj, double c);

// Collocation and Topology Building
int Collocation(bool* checker, Options* options, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors);
int SVDecomposition(std::vector<std::vector<double>> &A, std::vector<double> &W, std::vector<std::vector<double>> &V);
int LUDecomposition(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, double &D);
int LUBackSubstitution(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, std::vector<double> &BV);

// Solution Methods
int Solver(bool* checker, InputData* inputData, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData);
int Solution(bool* checker, InputData* inputData, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData);
