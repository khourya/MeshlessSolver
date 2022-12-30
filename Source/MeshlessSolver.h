#pragma once

#include "Parameters.h"
#include "Structures.h"

// Commonly used character arrays
const char tab[2] = "\t";
const char endl[2] = "\n";

// Unit Tests
bool RunUnitTests();

// Reading Inputs
void InputChecker(InputData* inputData);
int ReadInputs(bool* checker, InputData* inputData);

// Reading Options
void OptionsChecker(nodeStratFlag* nodeGenFlag, collocationStratFlag* collocationFlag);
int  ReadOptions(bool* checker, Options* options);

// Preprocessor Functions
int  PreprocessDomain(bool* checker, Options* options, InputData* inputData, PreprocessorData* preProcData, SolutionData* solution);
void SetInitialCondition(int nB, int nI, SolutionData* solution);
bool isInsideDomain(double xc, double yc, PreprocessorData* preProcData);
bool isOnEdge(double xi, double yi, PreprocessorData* preProcData);
void LogBoundaryPoints(int i, int j, int nBoundaryPoints);
void LogDomainExtents(double xmin, double xmax, double ymin, double ymax);

// Triangulation Procedure for Visualization
int Triangulate(bool* checker, PreprocessorData* preProcData);

// RBF and Polynomial Functions
double RBF(double x, double y, double xj, double yj, double c);
double RBF_dx(double x, double y, double xj, double yj, double c);
double RBF_dy(double x, double y, double xj, double yj, double c);
double RBF_d2(double x, double y, double xj, double yj, double c);

// Collocation and Topology Building
int Collocation(bool* checker, Options* options, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors);
int SVDecomposition(std::vector<std::vector<double>> &A, std::vector<double> &W, std::vector<std::vector<double>> &V);
int LUDecomposition(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, double &D);
int LUBackSubstitution(std::vector<std::vector<double>> &A, int nInfluencePoints, std::vector<int> &index, std::vector<double> &BV);

// Solution Methods
int Solver(bool* checker, InputData* inputData, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData);
int Solution(bool* checker, InputData* inputData, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData);

// Logging Output Data
void WriteOutput(double step, double time, PreprocessorData* preProcData, SolutionData* solutionData);
void WriteCSV(double NB, std::vector<double> col1, std::vector<double> col2);
void WriteMatrix(std::vector<std::vector<double>> A, std::vector<const char*> headers);
void WriteShapeFactors(std::vector<double> factors, std::vector<double> shapeFactors, std::vector<int> nInfluencePoints);
void LogErrors(int errorFlag);

// Writing out Matrices to Console for Viewing
void LogMat(const char* c, std::vector<double> A);               // Viewing 1D Vectors
void LogMat(const char* c, std::vector<int> A);                  // Viewing 1D Vectors
void LogMat(const char* c, std::vector<std::vector<double>> A);  // Viewing 2D Vectors
void LogMat(const char* c, std::vector<std::vector<int>> A);     // Viewing 2D Vectors