#include "MeshlessSolver.h"

// Standard Local Collocation Methods
void StdLocalCollocation::displayStrat()
{
	std::cout << "Implementing standard local collocation." << endl;
}

int StdLocalCollocation::Topology(bool* checker, GeometricData* preProcData, CollocationData* collocationData)
{
	int errorFlag = 0;

	// Radius of Influence Topology
	double influenceRadius = 2.2 * std::sqrt(preProcData->delX * preProcData->delX + preProcData->delY * preProcData->delY);  // Radius of Influence Topology

	// Capture Points within the Topology of Influence
	for (int i = 0; i < preProcData->nInternalPoints; i++)
	{
		// Select a point in the domain, "i" and set the initial number of points within its topology of inluence as zero
		collocationData->nInfluencePoints.push_back(0);
		std::vector<int> topologyRow;  // a row of indices that will be pushed back into the iTopology vector
		for (int j = 0; j < preProcData->nInternalPoints; j++)
		{
			// Measure distance from each point "j," to point of interest, "i"
			double dist = (preProcData->Xc[i] - preProcData->Xc[j]) * (preProcData->Xc[i] - preProcData->Xc[j]) + (preProcData->Yc[i] - preProcData->Yc[j]) * (preProcData->Yc[i] - preProcData->Yc[j]);
			dist = std::sqrt(dist);
			// If within the topology of influence relative to center point, "i", then add the point to the list!
			if ((dist < influenceRadius) && (collocationData->nInfluencePoints[i] < NFMax))
			{
				collocationData->nInfluencePoints[i] = collocationData->nInfluencePoints[i] + 1;
				topologyRow.push_back(j);
			}
		}
		collocationData->iTopology.push_back(topologyRow);
	}
	// std::cout << preProcData->nInternalPoints;
	// Optimize RBF Shape Parameter on Every Topology
	for (int i = 0; i < preProcData->nInternalPoints; i++)
	{
		double factor = 5.;
		double cond = 1.;
		double condMin = 1.e12;
		double condMax = 1.e14;
		bool bOut = false;

		collocationData->c.push_back(0.);

		int ITER = 0;
		while (!bOut)
		{
			ITER++;
			collocationData->c[i] = factor * std::sqrt(preProcData->delX * preProcData->delX + preProcData->delY * preProcData->delY);
			// Collocation Matrix
			std::vector <std::vector<double> > A;

			for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
			{	
				int I = collocationData->iTopology[i][ii];
				double xi = preProcData->Xc[I];
				double yi = preProcData->Yc[I];
				std::vector<double> rowA;
				for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
				{
					int J = collocationData->iTopology[i][jj];
					double xj = preProcData->Xc[J];
					double yj = preProcData->Yc[J];
					
					rowA.push_back(RBF(xi, yi, xj, yj, collocationData->c[i]));
				}
				A.push_back(rowA);
			}

			// Singular Value Decomposition
			std::vector<double> W;
			std::vector<std::vector<double>> V;

			for (int k = 0; k < collocationData->nInfluencePoints[i]; k++)
			{
				// Instantiate W
				W.push_back(0.);

				// Instantiate V
				std::vector<double> tempRow(collocationData->nInfluencePoints[i], 0.);
				V.push_back(tempRow);
			}

			errorFlag = SVDecomposition(A, W, V);

			// Max and Min Singular Value and Condition Number
			double svMin = 1.e100;
			double svMax = -1.e100;

			for (int k = 0; k < collocationData->nInfluencePoints[i]; k++)
			{
				if (W[k] < svMin)
				{
					svMin = W[k];
				}
				if (W[k] > svMax)
				{
					svMax = W[k];
				}
			}
			double conditionNumber = svMax / svMin;

			if (conditionNumber < condMin)
			{
				factor = factor + 1.e-1;
			}
			if (conditionNumber > condMax)
			{
				factor = factor - 1.e-1;
			}

			if ((conditionNumber >= condMin) && (conditionNumber <= condMax))
			{
				bOut = true;
			}
			if (factor < 1.e-1)
			{
				bOut = true;
			}

			// std::cout << std::endl << "Data Center: " << i << "\tITER: " << ITER << ",\t Factor: " << factor << "\tSV's: [" << svMin << ", " << svMax << "],\t CondNumber: " << conditionNumber << std::endl;
		}
		if (*checker)
			std::cout << "Data Center: " << i << "\tITER: " << ITER << ",\t Factor: " << factor << std::endl;
		
		// Track factor evolution
		collocationData->factors.push_back(factor);
	}

	WriteShapeFactors(collocationData->factors ,collocationData->c, collocationData->nInfluencePoints);
	
	return errorFlag;
}

int StdLocalCollocation::Collocate(bool* checker, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors)
{
	int errorFlag = 0;
	
	for (int i = 0; i < preProcData->nInternalPoints; i++)
	{
		// Creating Phi Matrix
		std::vector<std::vector<double>> phiMatrix;

		for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
		{
			double xi = preProcData->Xc[collocationData->iTopology[i][ii]];
			double yi = preProcData->Yc[collocationData->iTopology[i][ii]];

			// Row of phiMatrix that we will push back on each iteration
			std::vector<double> phiRow;

			for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
			{
				double xj = preProcData->Xc[collocationData->iTopology[i][jj]];
				double yj = preProcData->Yc[collocationData->iTopology[i][jj]];
				phiRow.push_back(RBF(xi, yi, xj, yj, collocationData->c[i]));
			}
			phiMatrix.push_back(phiRow);
		}

		// Performing LU Decomposition on Phi Matrix
		double D = 0.;
		std::vector<int> index(collocationData->nInfluencePoints[i], 0);
		LUDecomposition(phiMatrix, collocationData->nInfluencePoints[i], index, D);
		
		// Invert Phi Matrix through LU Back Substitution
		std::vector<std::vector<double>> phiInverse;
		std::vector<double> BV(collocationData->nInfluencePoints[i], 0.);

		// Creating Empty phiInverse
		for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
		{
			std::vector<double> tempRow(collocationData->nInfluencePoints[i], 0.);
			phiInverse.push_back(tempRow);
		}
		
		for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
		{
			for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
			{
				BV[ii] = 0.;
			}
			BV[jj] = 1.;
			errorFlag = LUBackSubstitution(phiMatrix, collocationData->nInfluencePoints[i], index, BV);
			for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
			{
				phiInverse[ii][jj] = BV[ii];
			}
		}

		// Derivative Interpolation
		std::vector<double> dLc(collocationData->nInfluencePoints[i], 0.);  // temporary vector
		std::vector<double> dL2(collocationData->nInfluencePoints[i], 0.);  // temporary vector
		std::vector<double> dLx(collocationData->nInfluencePoints[i], 0.);  // temporary vector
		std::vector<double> dLy(collocationData->nInfluencePoints[i], 0.);  // temporary vector

		double xi = preProcData->Xc[i];
		double yi = preProcData->Yc[i];
		for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
		{
			double xj = preProcData->Xc[collocationData->iTopology[i][jj]];
			double yj = preProcData->Yc[collocationData->iTopology[i][jj]];
			dL2[jj] = RBF_d2(xi, yi, xj, yj, collocationData->c[i]);
			dLx[jj] = RBF_dx(xi, yi, xj, yj, collocationData->c[i]);
			dLy[jj] = RBF_dy(xi, yi, xj, yj, collocationData->c[i]);
		}

		// Creating row of 0's at row [i] in each interpolation vector
		std::vector<double> tempRow(collocationData->nInfluencePoints[i], 0.);
		interpolationVectors->d2.push_back(tempRow);
		interpolationVectors->dx.push_back(tempRow);
		interpolationVectors->dy.push_back(tempRow);
		interpolationVectors->dBc.push_back(tempRow);
		interpolationVectors->dBx.push_back(tempRow);
		interpolationVectors->dBy.push_back(tempRow);

		for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
		{
			for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
			{
				interpolationVectors->d2[i][jj] += dL2[ii] * phiInverse[ii][jj];
				interpolationVectors->dx[i][jj] += dLx[ii] * phiInverse[ii][jj];
				interpolationVectors->dy[i][jj] += dLy[ii] * phiInverse[ii][jj];
			}
		}

		// Boundary End-Node Interpolation
		if (i < preProcData->nBoundaryPoints)
		{
			double xi = preProcData->X[i][0];
			double yi = preProcData->Y[i][0];
			for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
			{
				double xj = preProcData->Xc[collocationData->iTopology[i][jj]];
				double yj = preProcData->Yc[collocationData->iTopology[i][jj]];
				dLc[jj] = RBF(xi, yi, xj, yj, collocationData->c[i]);
				dLx[jj] = RBF_dx(xi, yi, xj, yj, collocationData->c[i]);
				dLy[jj] = RBF_dy(xi, yi, xj, yj, collocationData->c[i]);
			}

			for (int jj = 0; jj < collocationData->nInfluencePoints[i]; jj++)
			{
				for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
				{
					interpolationVectors->dBc[i][jj] += dLc[ii] * phiInverse[ii][jj];
					interpolationVectors->dBx[i][jj] += dLx[ii] * phiInverse[ii][jj];
					interpolationVectors->dBy[i][jj] += dLy[ii] * phiInverse[ii][jj];
				}
			}
		}
	}

	return errorFlag;
}
