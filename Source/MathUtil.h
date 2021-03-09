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
double VolumeTruncatedRightTriangularPrism(double dx, double dy, double s1, double s2, double s3, double s4);
double VolumeTruncatedRightSquarePrism(double dx, double dy, double s1, double s2, double s3, double s4);
int Integrate(double dt, std::vector<double> x, std::vector<double> y, std::vector<double> s0, std::vector<double> s1, double & dResult);

// Vector Functions
std::vector<double> CrossProduct(std::vector<double> a, std::vector<double> b);
