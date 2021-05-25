/***********************************************************************************************************************
**
**
**      Some government copyright stuff
**
**
***********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------
/// \file InputProcesser.cpp
///
/// Contains the functions for reading data from the Meshless Input File, LCMMData the file that contains the data that
/// defines the computational domain. Also contains the "debugger" file, which cites the data read from the file to
/// ensure the processor is reading/processing data correctly.
//----------------------------------------------------------------------------------------------------------------------

#include "MeshlessSolver.h"

// Standard Generation Methods

void StandardGeneration::displayStrat()
{
	std::cout << "Implementing standard internal node generation method." << endl;
}

int StandardGeneration::generateNodes(bool* checker, GeometricData* geometricData)
{
	int errorFlag = 0;

	// Temporary varible representing max-min
	double spanX = geometricData->xmax - geometricData->xmin;
	double spanY = geometricData->xmax - geometricData->xmin;

	geometricData->m_nInternalPoints = 1 * geometricData->m_nBoundaryPoints;
	int nX = static_cast<int>(1001. * spanX / geometricData->delX) / 1000;
	int nY = static_cast<int>(1001. * spanY / geometricData->delY) / 1000;

	for (int i = 1; i < nY; i++)
	{
		for (int j = 1; j < nX; j++)
		{
			double xInternal = geometricData->xmin + (spanX * static_cast<double>(j) / static_cast<double>(nX));
			double yInternal = geometricData->ymin + (spanY * static_cast<double>(i) / static_cast<double>(nY));
			if (isOnEdge(xInternal, yInternal, geometricData))
			{
				continue;
			}
			if (isInsideDomain(xInternal, yInternal, geometricData))
			{
				bool closeToBoundary = false;
				for (int k = 0; k < geometricData->m_nBoundaryPoints; k++)
				{
					double dist = (xInternal - geometricData->Xc[k])*(xInternal - geometricData->Xc[k]) + (yInternal - geometricData->Yc[k])*(yInternal - geometricData->Yc[k]);
					dist = std::sqrt(dist);
					if (dist < geometricData->dL[k] / 3.)
					{
						closeToBoundary = true;
					}
				}
				// If not on the boundary, add to the vector of internal points
				if (!closeToBoundary) // && (!onEdge))
				{
					geometricData->m_nInternalPoints++;
					geometricData->Xc.push_back(xInternal);
					geometricData->Yc.push_back(yInternal);

					if (*checker)
						std::cout << "New Internal Point: [" << xInternal << ", " << yInternal << "]" << endl;
				}
			}
		}
	}

	return errorFlag;
}

// Ravel Generation Methods
void RavelGeneration::displayStrat()
{
	std::cout << "Implementing Ravel internal node generation method." << endl;
}

int RavelGeneration::generateNodes(bool* checker, GeometricData* preProcData)
{
	int errorFlag = 0;
	return errorFlag;
}

bool isInsideDomain(double xi, double yi, GeometricData* preProcData)
{
	bool inside = true;

	double Ci = 0.;
	for (int j = 0; j < preProcData->m_nBoundaryPoints; j++)
	{
		double diffX = preProcData->X[j][2] - xi;
		double diffY = preProcData->Y[j][2] - yi;
		double r2 = diffX * diffX + diffY * diffY;
		Ci = Ci + (diffX*preProcData->dNx[j] + diffY*preProcData->dNy[j]) * preProcData->dL[j] / r2;
	}

	Ci = Ci / (2. * pi);

	if (Ci < 0.75)
	{
		inside = false;
	}

	return inside;
}

bool isOnEdge(double xi, double yi, GeometricData* preProcData)
{
	bool onEdge = false;
	if (xi == preProcData->xmin)
	{
		onEdge = true;
	}
	else if (xi == preProcData->xmax)
	{
		onEdge = true;
	}
	else if (yi == preProcData->ymin)
	{
		onEdge = true;
	}
	else if (yi == preProcData->ymax)
	{
		onEdge = true;
	}

	return onEdge;
}