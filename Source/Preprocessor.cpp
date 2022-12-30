#include <iostream>

#include "MeshlessSolver.h"

int PreprocessDomain(bool* checker, Options* options, InputData* inputData, PreprocessorData* preProcData, SolutionData* solution)
{
	int errorFlag = 0;  // error reporting flag

	int nCorners = inputData->nSides;  // Number of corners of the solution domain [unitless]

	// Build boundary data
	double sumX = 0;  // total span of domain in x-direction?
	double sumY = 0;  // total span of domain in y-direction?

	// Parameters for generating boundary data points
	double s1, s2, s3;
	double x1, x2, x3;
	double y1, y2, y3;

	if (*checker)
	{
		std::cout << endl << "Logging number of boundary points: " << endl;
	}
		
	// Generating boundary data
	for (int i = 0; i < inputData->nSides; i++)
	{
		for (int j = 0; j < inputData->NPS[i]; j++)
		{
			preProcData->nBoundaryPoints++;
			if (*checker)
				LogBoundaryPoints(i, j, preProcData->nBoundaryPoints);
			
			s1 = -1. + 2. * (static_cast<double>(j + 1) - 1.)  / static_cast<double>(inputData->NPS[i]);
			s2 = -1. + 2. * (static_cast<double>(j + 1) - 0.5) / static_cast<double>(inputData->NPS[i]);
			s3 = -1. + 2. * (static_cast<double>(j + 1) - 0.)  / static_cast<double>(inputData->NPS[i]);

			x1 = inputData->XS[i][0] * (s1) * (s1 - 1.) / 2. +
				 inputData->XS[i][1] * (1. + s1) * (1. - s1) +
				 inputData->XS[i][2] * (s1) * (s1 + 1.) / 2.;
			y1 = inputData->YS[i][0] * (s1) * (s1 - 1.) / 2. +
				 inputData->YS[i][1] * (1. + s1) * (1. - s1) +
				 inputData->YS[i][2] * (s1) * (s1 + 1.) / 2.;
			
			x2 = inputData->XS[i][0] * (s2) * (s2 - 1.) / 2. +
                 inputData->XS[i][1] * (1. + s2) * (1. - s2) +
				 inputData->XS[i][2] * (s2) * (s2 + 1.) / 2.;
			y2 = inputData->YS[i][0] * (s2) * (s2 - 1.) / 2. +
				 inputData->YS[i][1] * (1. + s2) * (1. - s2) +
				 inputData->YS[i][2] * (s2) * (s2 + 1.) / 2.;

			x3 = inputData->XS[i][0] * (s3) * (s3 - 1.) / 2. +
				 inputData->XS[i][1] * (1. + s3) * (1. - s3) +
				 inputData->XS[i][2] * (s3) * (s3 + 1.) / 2.;
			y3 = inputData->YS[i][0] * (s3) * (s3 - 1.) / 2. +
				 inputData->YS[i][1] * (1. + s3) * (1. - s3) +
				 inputData->YS[i][2] * (s3) * (s3 + 1.) / 2.;

			std::vector<double> xRow = { x1, x2, x3 };
			std::vector<double> yRow = { y1, y2, y3 };

			preProcData->X.push_back(xRow);
			preProcData->Y.push_back(yRow);

			double dX = x3 - x1;
			double dY = y3 - y1;
			double dL = std::sqrt(dX*dX + dY*dY);
			double dispX = std::abs(-dY / dL);
			double dispY = std::abs(dX / dL);

			preProcData->dL.push_back(dL);
			preProcData->dNx.push_back(dY / dL);
			preProcData->dNy.push_back(-dX / dL);
			preProcData->delX += dL * dispX;
			preProcData->delY += dL * dispY;
			sumX += dispX;
			sumY += dispY;

			double BC = inputData->BCS[i][0] * (s2) * (s2 - 1.) / 2. + inputData->BCS[i][1] * (1. + s2) * (1. - s2) + inputData->BCS[i][2] * (s2) * (s2 + 1.) / 2.;
			double HB = inputData->HBS[i][0] * (s2) * (s2 - 1.) / 2. + inputData->HBS[i][1] * (1. + s2) * (1. - s2) + inputData->HBS[i][2] * (s2) * (s2 + 1.) / 2.;

			preProcData->gamma1.push_back(0.);
			preProcData->gamma2.push_back(0.);
			preProcData->gamma3.push_back(BC);

			int NB = static_cast<int>(preProcData->nBoundaryPoints);

			switch (inputData->BoundaryType[i])
			{
			case 1:
				preProcData->gamma1[NB - 1] = 1.;
				preProcData->gamma2[NB - 1] = 0.;
				break;
			case 2:
				preProcData->gamma1[NB - 1] = 0.;
				preProcData->gamma2[NB - 1] = -inputData->D;
				break;
			case 3:
				preProcData->gamma1[NB - 1] = 1.;
				preProcData->gamma2[NB - 1] = inputData->D;
				break;
			}
		}
	} // end generating boundary data

	// Calculate Average Spacing
	preProcData->delX = 0.5 * preProcData->delX / sumX;
	preProcData->delY = 0.5 * preProcData->delY / sumY;

	// Calculating extents of domain (x-min/max and y-min/max)
	for (int i = 0; i < inputData->nSides; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Updated xmax
			if (inputData->XS[i][j] > preProcData->xmax)
			{
				preProcData->xmax = inputData->XS[i][j];
			}

			// Updated xmin
			if (inputData->XS[i][j] < preProcData->xmin)
			{
				preProcData->xmin = inputData->XS[i][j];
			}

			// Updated ymax
			if (inputData->YS[i][j] > preProcData->ymax)
			{
				preProcData->ymax = inputData->YS[i][j];
			}

			// Updated ymin
			if (inputData->YS[i][j] < preProcData->ymin)
			{
				preProcData->ymin = inputData->YS[i][j];
			}

		}
	}
	if (*checker)
		LogDomainExtents(preProcData->xmin, preProcData->xmax,
			             preProcData->ymin, preProcData->ymax);

	// Distribute Boundary, Normal Layer and Internal Points
	// Boundary Nodes
	for (int i = 0; i < preProcData->nBoundaryPoints; i++)
	{
		preProcData->Xc.push_back(preProcData->X[i][1]);
		preProcData->Yc.push_back(preProcData->Y[i][1]);
	}

	// Corner Nodes, I think unnecessary
	// for (int i = 0; i < preProcData->nCornerPoints; i++)
	// {
	// 	preProcData->Xcp.push_back(inputData->XS[i][3]);
	// 	preProcData->Ycp.push_back(inputData->YS[i][3]);
	// }

	// Internal Layer of Nodes Normal to Boundary nodes
	for (int i = 0; i < preProcData->nBoundaryPoints; i++)
	{	
		preProcData->Xc.push_back(preProcData->X[i][1] - preProcData->dL[i] * preProcData->dNx[i] / 4.);
		preProcData->Yc.push_back(preProcData->Y[i][1] - preProcData->dL[i] * preProcData->dNy[i] / 4.);
	}

	// Generating Internal Nodes
	NodeGenerator nodeGenerator(options->internalNodeGenFlag);
	if (*checker)
		nodeGenerator.m_nodeStrategy->displayStrat();

	if (nodeGenerator.m_nodeStrategy == nullptr)
	{
		std::cout << endl << "Internal node generation strategy pointer is null." << endl;
		errorFlag = -3000;
		return errorFlag;
	}

	errorFlag = nodeGenerator.m_nodeStrategy->generateNodes(checker, preProcData);

	// Shift Boundary Points and Normals to Element End-Node
	for (int j = 0; j < preProcData->nBoundaryPoints; j++)
	{
		preProcData->Xc[j] = preProcData->X[j][0];
		preProcData->Yc[j] = preProcData->Y[j][0];
		double distMin = 1.e10;
		for (int i = 0; i < preProcData->nBoundaryPoints; i++)
		{
			int iMin = 0;
			double distX = preProcData->Xc[j] - preProcData->X[i][2];
			double distY = preProcData->Yc[j] - preProcData->Y[i][2];
			double dist = std::sqrt(distX * distX + distY * distY);
			if (dist < distMin)
			{
				distMin = dist;
				iMin = i;
			}
			double dNxt_t_local = (preProcData->dNx[j] + preProcData->dNx[iMin]) / 2.;
			double dNyt_t_local = (preProcData->dNy[j] + preProcData->dNy[iMin]) / 2.;
			double d2 = std::sqrt(dNxt_t_local * dNxt_t_local + dNyt_t_local * dNyt_t_local);
			preProcData->dNx_t.push_back(dNxt_t_local/d2);
			preProcData->dNy_t.push_back(dNyt_t_local/d2);
		}
	}

	// Perform Triangulation of the Point Distribution
	errorFlag = Triangulate(checker, preProcData);

	// Shift boundary points back to element center
	for (int i = 0; i < preProcData->nBoundaryPoints; i++)
	{
		preProcData->Xc[i] = preProcData->X[i][1];
		preProcData->Yc[i] = preProcData->Y[i][1];
	}

	// Prescribe Initial Condition
	SetInitialCondition(preProcData->nBoundaryPoints, preProcData->nInternalPoints, solution);

	WriteCSV(preProcData->nBoundaryPoints, preProcData->Xc, preProcData->Yc);
	return errorFlag;
}

void SetInitialCondition(int nB, int nI, SolutionData* solutionData)
{
	// Empty vector of size nB and nI
	std::vector<double> sizeNB(nB, 0.);
	std::vector<double> sizeNI(nI, 0.);

	// Boundary Points
	   // Unbound and Bound Concentration
	solutionData->cT_bnd = sizeNB;
	solutionData->cB_bnd = sizeNB;

	   // Unbound Flux
	solutionData->fTx_bnd = sizeNB;
	solutionData->fTy_bnd = sizeNB;

	// Boundary Points
	// for (int i = 0; i < nB; i++)
	// {
	// 	// Unbound and Bound Concentration
	// 	solutionData->cT_bnd.push_back(0.);
	// 	solutionData->cB_bnd.push_back(0.);
	// 
	// 	// Unbound Flux
	// 	solutionData->fTx_bnd.push_back(0.);
	// 	solutionData->fTy_bnd.push_back(0.);
	// }

	// Internal Points
	   // Concentration
	solutionData->cT_int = sizeNI;
	solutionData->cB_int = sizeNI;

	   // Unbound Flux
	solutionData->fTx_int = sizeNI;
	solutionData->fTy_int = sizeNI;

	   // Laplace
	solutionData->D2T = sizeNI;

	   // Next Solutions
	solutionData->c_next = sizeNI;
	solutionData->cb_next = sizeNI;

	// for (int i = 0; i < nI; i++)
	// {
	// 	// Concentration
	// 	solutionData->cT_int.push_back(0.);
	// 	solutionData->cB_int.push_back(0.);
	// 
	// 	// Unbound Flux
	// 	solutionData->fTx_int.push_back(0.);
	// 	solutionData->fTy_int.push_back(0.);
	// 
	// 	// Laplace
	// 	solutionData->D2T.push_back(0.);
	// }
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Outputs the generation/indexing of boundary points to ensure proper functionality
///
/// \param[in] i                 Index of side we are currently iterating through
/// \param[in] j                 Index of boundary points on the side we are currently iterating through
/// \param[in] nBoundaryPoints   Total number of boundary points accumulated
///
/// return                 <none>
///
/// _PUT_DETAILED_DESCRIPTION_HERE_
//----------------------------------------------------------------------------------------------------------------------
void LogBoundaryPoints(int i, int j, int nBoundaryPoints)
{
	std::cout << "Side: " << i + 1 << "," << tab << "Boundary Point: " << j + 1 << "," << tab << "Total Boundary Points: " << nBoundaryPoints << endl;
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Outputs the x-coordinate and y-coordinate extents of the domain to ensure proper functionality
///
/// \param[in] xmin   Current minimum of x-coordinate
/// \param[in] xmax   Current maximum of x-coordinate
/// \param[in] ymin   Current minimum of y-coordinate
/// \param[in] ymax   Current maximum of y-coordinate
///
/// return            <none>
///
/// _PUT_DETAILED_DESCRIPTION_HERE_
//----------------------------------------------------------------------------------------------------------------------
void LogDomainExtents(double xmin, double xmax, double ymin, double ymax)
{
	std::cout << endl << "Logging Domain Extents:" << endl;
	std::cout << "X = [" << xmin << ", " << xmax << "]" << tab;
	std::cout << "Y = [" << ymin << ", " << ymax << "]" << endl;
}