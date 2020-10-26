#include "MeshlessSolver.h"

int Solver(bool* checker, InputData* inputData, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData)
{
	int errorFlag = 0;

	// to = inputData->initialTime;
	// tf = inputData->finalTime;
	// dt = inputData->deltaTime;
	// nT = inputData->nSteps;

	// Initializing the solution size
	// std::vector<double> c(NN, 0.);
	// std::vector<double> cb(NN, 0.);
	// solutionData->c_next = c;
	// solutionData->cb_next = cb;

	// Writing Initial Condition
	std::cout << "     Writing Solution:   Time: " << 0 << " of " << inputData->finalTime << "\t|   Step: " << 0 << " of " << inputData->nSteps << std::endl;
	WriteOutput(0, 0, preProcData, solutionData);

	// Iterating through solution
	for (int N = 1; N <= inputData->nSteps; N++)
	{
		errorFlag = Solution(checker, inputData, preProcData, collocationData, interpolationVectors, solutionData);

		double t = N*inputData->deltaTime + inputData->initialTime;

		if (N % inputData->outInterval == 0)
		{
			std::cout << "     Writing Solution:   Time: " << t << " of " << inputData->finalTime << "\t|   Step: " << N << " of " << inputData->nSteps << std::endl;
			WriteOutput(N, t, preProcData, solutionData);
		}
	}

	return errorFlag;
}

int Solution(bool* checker, InputData* inputData, PreprocessorData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors, SolutionData* solutionData)
{
	int errorFlag = 0;

	int NN = preProcData->nInternalPoints;
	int NB = preProcData->nBoundaryPoints;
	int diffN = NN - NB;

	// Temporary vector to store solution at next time step
	// std::vector<double> c(NN, 0.);
	// std::vector<double> cb(NN, 0.);
	// solutionData->c_next = c;
	// solutionData->cb_next = cb;

	// Constants
	double dt = inputData->deltaTime;  // dt [s]
	double D = inputData->D;  // diffusion coefficient [cm/s2]
	double Km = inputData->Km;  // [1/s]
	double Ke = inputData->Ke;  // [1/s]
	double K12 = inputData->K12;  // [1/s]
	double K21 = inputData->K21;  // [1/s]
	double Vb = inputData->Vb;
	double Vt = inputData->Vt;
	double vb = Vb / Vt;

	// Calculate Internal Concentrations
	for (int i = NB; i < NN; i++)
	{
		solutionData->c_next[i] = solutionData->cT_int[i] + dt * (D * solutionData->D2T[i] - (K21 + Km) * solutionData->cT_int[i] + (K12 * Vb / Vt) * solutionData->cB_int[i]);
		solutionData->cb_next[i] = solutionData->cB_int[i] + dt * ((K21 * Vt / Vb) * solutionData->cT_int[i] - (K12 + Ke) * solutionData->cB_int[i]);
	}

	// Calculate Boundary Concentrations using Internal Normal Layer
	for (BoundaryObject* boundary : preProcData->Boundaries)
	{
		boundary->ApplyBoundaryCondition(preProcData, solutionData);
	}

	// for (int i = 0; i < NB; i++)
	// {
	// 	double g1 = preProcData->gamma1[i];
	// 	double g2 = preProcData->gamma2[i];
	// 	double g3 = preProcData->gamma3[i];
	// 
	// 	double range = preProcData->dL[i] / 4.;
	// 	solutionData->c_next[i] = (g3 * range + g2 * solutionData->c_next[NB + i]) / (g1 * range + g2);
	// 	solutionData->cb_next[i] = (g3 * range + g2 * solutionData->cb_next[NB + i]) / (g1 * range + g2);
	// }

	// Update Concentration Vector
	for (int i = 0; i < NN; i++)
	{
		// start coding
		solutionData->cT_int[i] = solutionData->c_next[i];
		solutionData->cB_int[i] = solutionData->cb_next[i];
	}

	// Calculate Concentration Derivatives
	for (int i = 0; i < NN; i++)
	{
		solutionData->fTx_int[i] = 0.;
		solutionData->fTy_int[i] = 0.;
		solutionData->D2T[i] = 0.;
		for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
		{
			int j = collocationData->iTopology[i][ii];
			solutionData->fTx_int[i] += interpolationVectors->dx[i][ii] * solutionData->cT_int[j];
			solutionData->fTy_int[i] += interpolationVectors->dy[i][ii] * solutionData->cT_int[j];
			solutionData->D2T[i] += interpolationVectors->d2[i][ii] * solutionData->cT_int[j];
		}
		solutionData->fTx_int[i] *= -inputData->D;
		solutionData->fTy_int[i] *= -inputData->D;
	}

	// Calculate boundary end-node concentrations and fluxes
	for (int i = 0; i < NB; i++)
	{
		// Unbound
		solutionData->cT_bnd[i] = 0.;
		solutionData->fTx_bnd[i] = 0.;
		solutionData->fTy_bnd[i] = 0.;

		// Bound
		solutionData->cB_bnd[i] = 0.;

		for (int ii = 0; ii < collocationData->nInfluencePoints[i]; ii++)
		{
			int j = collocationData->iTopology[i][ii];
			// Unbound
			solutionData->cT_bnd[i] += interpolationVectors->dBc[i][ii] * solutionData->cT_int[j];
			solutionData->fTx_bnd[i] += interpolationVectors->dBx[i][ii] * solutionData->cT_int[j];
			solutionData->fTy_bnd[i] += interpolationVectors->dBy[i][ii] * solutionData->cT_int[j];

			// Bound
			solutionData->cB_bnd[i] += interpolationVectors->dBc[i][ii] * solutionData->cB_int[j];
		}

		// Unbound
		solutionData->fTx_bnd[i] *= -inputData->D;
		solutionData->fTy_bnd[i] *= -inputData->D;
	}

	return errorFlag;
}