/***********************************************************************************************************************
**
**
**      Some government copyright stuff
**
**
***********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------
/// \file NodeGenStrategy.h
///
/// Contains the functions for reading data from the Meshless Input File, LCMMData the file that contains the data that
/// defines the computational domain. Also contains the "debugger" file, which cites the data read from the file to
/// ensure the processor is reading/processing data correctly.
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Structures.h"

class CollocationStrategy
{
public:
	virtual ~CollocationStrategy() {};
	virtual int Collocate(bool* checker, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors) = 0;
	virtual int Topology(bool* checker, GeometricData* preProcData, CollocationData* collocationData) = 0;
	virtual void displayStrat() = 0;

};

class StdLocalCollocation : public CollocationStrategy
{
public:
	void displayStrat() override;
	int Collocate(bool* checker, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors) override;
	int Topology(bool* checker, GeometricData* preProcData, CollocationData* collocationData) override;
};

class PAugLocalCollocation : public CollocationStrategy
{
public:
	void displayStrat() override;
	int Collocate(bool* checker, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors) override;
	int Topology(bool* checker, GeometricData* preProcData, CollocationData* collocationData) override;
};

class GlobalCollocation : public CollocationStrategy
{
public:
	void displayStrat() override;
	int Collocate(bool* checker, GeometricData* preProcData, CollocationData* collocationData, InterpolationVectors* interpolationVectors) override;
	int Topology(bool* checker, GeometricData* preProcData, CollocationData* collocationData) override;
};

class CollocationMethod
{
public:
	CollocationMethod(collocationStratFlag collocationStrat) : m_stratFlag{ collocationStrat }
	{
		if (m_stratFlag == collocationStratFlag::StdLocal)
		{
			std::unique_ptr<CollocationStrategy> strategy(new StdLocalCollocation);
			m_collocationStrategy = std::move(strategy);
		}
		else if (m_stratFlag == collocationStratFlag::Global)
		{
			std::unique_ptr<CollocationStrategy> strategy(new GlobalCollocation);
			m_collocationStrategy = std::move(strategy);
		}
		else if (m_stratFlag == collocationStratFlag::LocalwPoly)
		{
			std::unique_ptr<CollocationStrategy> strategy(new PAugLocalCollocation);
			m_collocationStrategy = std::move(strategy);
		}
		else
		{
			std::unique_ptr<CollocationStrategy> strategy(new StdLocalCollocation);
			m_collocationStrategy = std::move(strategy);
		}
	}

	~CollocationMethod()
	{
	}

	std::unique_ptr<CollocationStrategy> m_collocationStrategy = nullptr;

private:
	collocationStratFlag m_stratFlag = collocationStratFlag::StdLocal;
};

