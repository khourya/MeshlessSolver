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

class NodeGenStrategy
{
public:
	virtual ~NodeGenStrategy() {};
	virtual int generateNodes(bool* checker, PreprocessorData* preProcData) = 0;
	virtual void displayStrat() = 0;

};

class StandardGeneration : public NodeGenStrategy
{
public:
	void displayStrat() override;
	int generateNodes(bool* checker, PreprocessorData* preProcData) override;
};

class RavelGeneration : public NodeGenStrategy
{
public:
	void displayStrat() override;
	int generateNodes(bool* checker, PreprocessorData* preProcData) override;
};

class NodeGenerator
{
public:
	NodeGenerator(nodeStratFlag nodeStrat) : m_stratFlag{ nodeStrat }
	{
		if (m_stratFlag == nodeStratFlag::Standard)
		{
			std::unique_ptr<NodeGenStrategy> strategy(new StandardGeneration);
			m_nodeStrategy = std::move(strategy);
		}
		else if (m_stratFlag == nodeStratFlag::Ravel)
		{
			std::unique_ptr<NodeGenStrategy> strategy(new RavelGeneration);
			m_nodeStrategy = std::move(strategy);
		}
		else
		{
			std::unique_ptr<NodeGenStrategy> strategy(new StandardGeneration);
			m_nodeStrategy = std::move(strategy);
		}
	}

	~NodeGenerator()
	{
	}

	std::unique_ptr<NodeGenStrategy> m_nodeStrategy = nullptr;

private:
	nodeStratFlag m_stratFlag = nodeStratFlag::Standard;
};

