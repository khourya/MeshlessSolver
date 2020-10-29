#pragma once
#include <vector>

// Writing out Matrices to Console for Viewing
void LogMat(const char* c, std::vector<double> A);               // Viewing 1D Vectors
void LogMat(const char* c, std::vector<int> A);                  // Viewing 1D Vectors
void LogMat(const char* c, std::vector<std::vector<double>> A);  // Viewing 2D Vectors
void LogMat(const char* c, std::vector<std::vector<int>> A);     // Viewing 2D Vectors

// Matrix Math
int InvertMatrix();

// Math Functions
int Integrate(double dt, std::vector<double> x, std::vector<double> y, std::vector<double> f0, std::vector<double> f1, double& dMass);
