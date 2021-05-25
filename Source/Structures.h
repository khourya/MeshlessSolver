#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include "MathUtil.h"
#include "Triangulate.h"

class GeometricData;
struct SolutionData;

// Enumerator for Node Generation Strategy
enum class nodeStratFlag
{
	Standard = 0,
	Jure,
	Ravel
};

// Enumerator for Topology/Collocation Strategy
enum class collocationStratFlag
{
	StdLocal = 0,
	LocalwPoly,
	Global
};

enum class PointClassification
{
	Undefined = -1,
	Boundary,
	Interior
};

enum class boundaryType
{
	FirstKind = 1,
	SecondKind,
	ThirdKind,
	FiniteVehicle
};

// Geometric Structures
struct Point
{
	Point(double x, double y) : m_x{ x }, m_y{ y }
	{};

	void SetIndex(int index) { m_index = index; }
	int GetIndex() { return m_index; }

	PointClassification GetPointClassification() { return m_pointType; }
	void SetPointClassification(PointClassification type) { m_pointType = type; }

	double GetX() { return m_x; }
	double GetY() { return m_y; }

	void SetBoundConcentration(double c) { m_cbound = c; }
	void SetUnboundConcentration(double c) { m_cfree = c; }
	double GetBoundConcentration() { return m_cbound; }
	double GetUnboundConcentration() { return m_cfree; }
private:
	// Indexing
	int m_index = -1;
	PointClassification m_pointType = PointClassification::Undefined;

	// Geometric Location
	double m_x = DBL_MIN;
	double m_y = DBL_MIN;

	// Concentrations
	double m_cbound = 0.;
	double m_cfree = 0.;
};

class Triangle
{
public:
	Triangle(Point* point1, Point* point2, Point* point3) : m_point1{ point1 }, m_point2{ point2 }, m_point3{ point3 }
	{}

	Point* GetPoint(int index);

	double GetArea();
	double GetVolume(std::string compartment);

	void SetIndex(int i) { m_index = i; }

private:
	Point* m_point1 = nullptr;
	Point* m_point2 = nullptr;
	Point* m_point3 = nullptr;
	int m_index = -1;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Options: Struct containing the information collected/read from the LCMMOptions.inp input options file
///
//----------------------------------------------------------------------------------------------------------------------
struct Options
{
	nodeStratFlag internalNodeGenFlag = nodeStratFlag::Standard;
	collocationStratFlag topologyGenFlag = collocationStratFlag::StdLocal;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// InputData: Struct containing the information collected/read from the LCMMData.inp input file
///
//----------------------------------------------------------------------------------------------------------------------
struct InputData
{
	// Cofficients
	double D = 0.;    // Diffusion coefficient [cm^2/s]
	double Km = 0.;   // Metabolic Rate Coefficient [1/s]
	double Ke = 0.;   // Elimination Rate Coefficient [1/s]
	double K12 = 0.;  // Binding Rate Coefficient [1/s]
	double K21 = 0.;  // Unbinding Rate Coefficient [1/s]
	double Vt = 0.;   // Tissue Volume [mL]
	double Vb = 0.;   // Blood Volume [mL]

	// Time Iteration Data
	double initialTime;    // Initial time (t_o) of the solution [s],  t = to:dt:tf
	double finalTime;      // Final time (t_f) of the solution [s],  t = to:dt:tf
	double deltaTime;  // Time step (dt) of the solution [s], t = to:dt:tf
	int nSteps;            // Number of time steps (N) to iterate during solution  [unitless], dt = (tf-to)/N
	int outInterval;       // Number of time steps between solutions that we want output/written [unitless]

	// Geometry Data
	int nSides = 0;  // Number of sides of the solution domain [unitless]

	std::vector<int> NPS;                   // Number of data centers per side of the domain, specified for each side
	std::vector <std::vector<double> > XS;  // x-locations 
	std::vector <std::vector<double> > YS;  // y-locations

	std::vector<int> BoundaryType;          // Boundary Kind: First (1), Second (2), Third (3)
	std::vector <std::vector<double> > BCS; // Boundary Value, Either: T_hat, q_hat or Tinfinity_hat 
	std::vector <std::vector<double> > HBS; // Film Coefficient: T + (k/h)*dT_dn = Tinf. HBS = h, k = diffusion coefficient
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Options: Class defining the boundary conditions and containing the functions for implementing BC's
/// 
//----------------------------------------------------------------------------------------------------------------------
class BoundaryObject
{
public:
	BoundaryObject(int boundaryKind, int NS, double diffusionCoefficient, double boundaryValue, double vehicleVolume);

	int ApplyBoundaryCondition(GeometricData* preProcData, SolutionData* solutionData);
	int UpdateDonorVolume(double dT, GeometricData* preProcData, SolutionData* solutionData);
	void AddNode(int index);
	void AddNormalVectors(double dNx, double dNy);

private:
	boundaryType m_boundaryKind = boundaryType::FirstKind;
	std::vector<int> m_boundaryMemberIndices;

	double m_volume = 0.;

	double m_gamma1 = 0.;
	double m_gamma2 = 0.;
	double m_gamma3 = 0.;

	std::vector<double> m_dNx;
	std::vector<double> m_dNy;
	std::vector<double> m_jDotN_old;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// PreprocesserData: Struct containing the information that defines the data centers of the domain
///
//----------------------------------------------------------------------------------------------------------------------
class GeometricData
{
public:
	void AddPoint(Point* point) { m_points.push_back(point); }

	std::vector<double> GetOrderedCoordinates();
	Point* GetPointByCoordinates(double x, double y);
	int GetPointsListLength() { return static_cast<int>(m_points.size()); }

	int GeneratePointsList();
	int GenerateTriangles(delaunator::Delaunator d);
	int WriteMeshData();

	std::vector<Point*> GetBoundaryPoints();
	std::vector<Point*> GetInteriorPoints();

	// Point Count
	int m_nBoundaryPoints = -1;               // Number of boundary points: NB
	int m_nInternalPoints = -1;               // Number of internal points: NN

	// Boundary Geometry
	std::vector <std::vector<double> > X;  // x-locations of boundary points
	std::vector <std::vector<double> > Y;  // y-locations of boundary points

	// Boundary Normal
	std::vector<double> dL;                // differential lengths for each boundary segment
	std::vector<double> dNx;               // unit normal for boundary segment in x-direction
	std::vector<double> dNy;               // unit normal for boundary segment in y-direction

	// Triangulation Parameters
	std::vector<double> dNx_t;             // Vector Variable for Triangulation 
	std::vector<double> dNy_t;             // Vector Variable for Triangulation

	// Boundary Condition
	/*
	Where: gamma1*T + gamma2*dT/dn = gamma3;

	All boundary types:
		gamma3 = BC value (T, q, or Tinf)

	1st Kind: T = BC
		gamma1 = 1;
		gamma2 = 0;

	2nd Kind: -k * dT/dn = BC
		gamma1 = 0;
		gamma2 = -D or -k; (negative of diffusion coefficient)

	3rd Kind: T + (k/h)*dT/dn = BC    or    -k*dT/dn = h(T - Tinf), where Tinf = BC
		gamma1 = 1;
		gamma2 = D or k/h; (diffusion coefficient / film coefficient)
	*/
	std::vector<double> gamma1;                 // Gamma1, specifying boundary type
	std::vector<double> gamma2;                 // Gamma2, specifying boundary type
	std::vector<double> gamma3;                 // Gamma3, specifying boundary type
	std::vector<BoundaryObject*> Boundaries; // vector containing the boundary objects

	// Geometry, Location of Data Centers
	std::vector<double> Xc;                // x-coordinate for Data Centers
	std::vector<double> Yc;                // y-coordinate for Data Centers
	double xmin = DBL_MAX;                 // lower bound on domain x-direction, initialized large and updated
	double xmax = DBL_MIN;                // upper bounds on domain x-direction, initialized small and updated
	double ymin = DBL_MAX;                 // lower bound on domain y-direction, initialized large and updated
	double ymax = DBL_MIN;                // upper bounds on domain x-direction, initialized small and updated

	// std::vector<double> Xcp;            // x-coordinate for Corner Points
	// std::vector<double> Ycp;            // y-coordinate for Corner Points

	// RBF Parameters
	double delX = 0;        // Average spacing in x-coordinate
	double delY = 0;        // Average spacing in y-coordinate

private:
	std::vector<Point*> m_points;
	std::vector <Triangle*> m_triangles;
};

struct SolutionData
{
	// Concentration - C
	std::vector<double> cT_bnd;   // field solution for the tissue concentration at the boundary
	std::vector<double> cT_int;   // field solution for the tissue concentration interior to the boundary
	std::vector<double> cB_bnd;   // field solution for the bound concentration at the boundary
	std::vector<double> cB_int;   // field solution for the bound concentration interior to the boundary

	// Flux - F
	std::vector<double> fTx_bnd;  // flux in the unbound concentration in the x-direction at the boundary
	std::vector<double> fTy_bnd;  // flux in the unbound concentration in the y-direction at the boundary
	std::vector<double> fTx_int;  // flux in the unbound concentration in the x-direction interior to the boundary
	std::vector<double> fTy_int;  // flux in the unbound concentration in the y-direction interior to the boundary

	// Laplace - D2
	std::vector<double> D2T;      // Laplace of the unbound concentration

	// Next Solution
	std::vector<double> c_next;
	std::vector<double> cb_next;
};

struct CollocationData
{
	std::vector<int> nInfluencePoints;          // Number of points that influence for each point in the domain
	std::vector <std::vector<int> > iTopology;  // Number of points that influence for each point in the domain
	std::vector<double> c;                      // Shape Factor for each topology
	std::vector<double> factors;                // Factor that constitutes the shape factor
};

struct InterpolationVectors
{
	// Internal Interpolation Vectors
	std::vector<std::vector<double>> d2;
	std::vector<std::vector<double>> dx;
	std::vector<std::vector<double>> dy;

	// Boundary Interpolation Vectors
	std::vector<std::vector<double>> dBc;
	std::vector<std::vector<double>> dBx;
	std::vector<std::vector<double>> dBy;
};