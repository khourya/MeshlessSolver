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

#include <iostream>

#include "MeshlessSolver.h"

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Reads input data from LCMMData.inp
///
/// \param[in] checker     Boolean to output data that we are reading to command line and run inputDebugger
/// \param[in] inputData   Pointer to the data structure containing the input data defining geometry and coefficients
///
/// return errorFlag       Integer flag tracking the errors that might be occurring during the reading process
///
/// _PUT_DETAILED_DESCRIPTION_HERE_
//----------------------------------------------------------------------------------------------------------------------
int ReadInputs(bool* checker, InputData* inputData)
{
    int errorFlag = 0;
    int nBoundaryPoints = 0;
    // Input processing
    std::string line;

    // Geometry and Boundary conditions
    int indexSide, nps, boundaryKind;
    double x1, x2, x3, y1, y2, y3;

    // Begin doing actual work
    std::ifstream inputFile;
    inputFile.open("Data\\LCMMData.inp");

    if (!inputFile) {
        std::cout << std::endl << "Unable to open the input file" << std::endl;
        errorFlag = -2000;   // call system to stop
        return errorFlag;
    }

    std::getline(inputFile, line);  // Header Line
    if (*checker)
    {
        // Validating options:
        std::cout << "  Validating Inputs:" << std::endl;
        std::cout << "\t" << line << std::endl;
    }

    std::getline(inputFile, line);  // Coefficient labels
    if (*checker)
        std::cout << "\t" << line << std::endl;

    inputFile >> inputData->D >> inputData->Km >> inputData->Ke >> inputData->K12 >> inputData->K21 >> inputData->Vt >> inputData->Vb >> inputData->Vd;  // Coefficient Values

    if (*checker)
        std::cout << "\t" << inputData->D << tab << inputData->Km << tab << inputData->Ke << tab << inputData->K12 << tab << inputData->K21 << tab << inputData->Vt << tab << inputData->Vb << tab << inputData->Vd << endl;
    
    std::getline(inputFile, line); // gets us back on track as the input >> inVariables thing doesn't count as a lineread

    std::getline(inputFile, line);  // Time operation labels

    if (*checker)
        std::cout  << "\t" << line << endl;

    inputFile >> inputData->initialTime >> inputData->finalTime >> inputData->nSteps >> inputData->outInterval;
    inputData->deltaTime = (inputData->finalTime - inputData->initialTime) / inputData->nSteps;

    if (*checker)
        std::cout << "\t" << inputData->initialTime << tab << inputData->finalTime << tab << inputData->nSteps << tab << inputData->outInterval << endl;

    std::getline(inputFile, line);  // gets us back on track as the input >> inVariables thing doesn't count as a lineread

    inputFile >> inputData->nSides;

    if (inputData->nSides > NSMax)
    {
        std::cout << endl << "Exceeded number of allowable sides for the domain geometry. (NSMax = " << NSMax << ")" << endl;
        errorFlag = -2001;
        return errorFlag;
    }

    if (*checker)
        std::cout << "\t" << "Number of Sides: " << inputData->nSides << endl;

    std::getline(inputFile, line);  // Domain Geometry Labels

    for (int i = 0; i < inputData->nSides; i++)
    {
        inputFile >> indexSide >> nps >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        if (*checker)
            std::cout  << "\t" << indexSide << tab << nps << tab << x1 << tab << y1 << tab << x2 << tab << y2 << tab << x3 << tab << y3 << endl;

        inputData->NPS.push_back(nps);
        nBoundaryPoints += nps;
        if (nBoundaryPoints > NBMax)
        {
            std::cout << endl << "Exceeded number of allowable boundary points. (NBMax = " << NBMax << ")" << endl;
            errorFlag = -2002;
            return errorFlag;
        }

        std::vector<double> xRow = { x1, x2, x3 };
        inputData->XS.push_back(xRow);

        std::vector<double> yRow = { y1, y2, y3 };
        inputData->YS.push_back(yRow);
    }

    std::getline(inputFile, line);  // gets us back on track as the input >> inVariables thing doesn't count as a lineread
    // std::getline(inputFile, line);  // Boundary Condition Labels

    if (*checker)
        std::cout << "\t" << line << endl;

    for (int i = 0; i < inputData->nSides; i++)
    {
        inputFile >> indexSide >> boundaryKind >> x1 >> y1 >> x2 >> y2 >> x3 >> y3; 
        if (*checker)
            std::cout << "\t" << indexSide << tab << boundaryKind << tab << x1 << tab << y1 << tab << x2 << tab << y2 << tab << x3 << tab << y3 << endl;


        if (boundaryKind != 1 && boundaryKind != 2 && boundaryKind != 3)
        {
            std::cout << endl << "Invalid Boundary Condition Type." << endl << "Input Kind: " << boundaryKind << ", at side: " << indexSide << endl;
            errorFlag = -2003;
            return errorFlag;
        }
        inputData->BoundaryType.push_back(boundaryKind);

        std::vector<double> BVRow = { x1, x2, x3 };
        inputData->BCS.push_back(BVRow);

        std::vector<double> HRow = { y1, y2, y3 };
        inputData->HBS.push_back(HRow);
    }

    inputFile.close();

    if (*checker)
    {
        InputChecker(inputData);
    }

    return errorFlag;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief
/// Outputs data read from LCMMData.inp file to ensure proper functionality
///
/// \param[in] inputData   Pointer to the data structure containing the input data defining geometry and coefficients
///
/// return                 <none>
///
/// _PUT_DETAILED_DESCRIPTION_HERE_
//----------------------------------------------------------------------------------------------------------------------
void InputChecker(InputData* inputData)
{
    std::cout << std::endl;
    std::cout << "  Checker for input file: " << std::endl;
    std::cout << "\tVt:\t" << inputData->Vt << std::endl;
    std::cout << "\tVb:\t" << inputData->Vb << std::endl;
    std::cout << "\tVd:\t" << inputData->Vd << std::endl;
    std::cout << "\tnOut:\t" << inputData->outInterval << std::endl;
    std::cout << "\tnSides:\t" << inputData->nSides << std::endl;
    std::cout << std::endl;

    std::cout << "\tSize of XS:\t" << inputData->XS.size() << endl;
    std::cout << "\tSize of YS:\t" << inputData->YS.size() << endl;
    std::cout << "\tSize of BCS:\t" << inputData->BCS.size() << endl;
    std::cout << "\tSize of HBS:\t" << inputData->HBS.size() << endl;
    std::cout << std::endl;

    std::cout << "\tXS: " << inputData->XS.size() << endl;
    for (int i = 0; i < inputData->XS.size(); i++)
    {
        std::cout << "\t  Side " << i + 1 << ": ";
        std::cout << inputData->XS[i][0] << ", " << inputData->XS[i][1] << ", " << inputData->XS[i][2] << endl;
    }
    std::cout << std::endl;

    std::cout << "\tYS: " << inputData->XS.size() << endl;
    for (int i = 0; i < inputData->YS.size(); i++)
    {
        std::cout << "\t  Side " << i + 1 << ": ";
        std::cout << inputData->YS[i][0] << ", " << inputData->YS[i][1] << ", " << inputData->YS[i][2] << endl;
    }
    std::cout << std::endl;

    std::cout << "\tBCS: " << inputData->XS.size() << endl;
    for (int i = 0; i < inputData->BCS.size(); i++)
    {
        std::cout << "\t  Side " << i + 1 << ": ";
        std::cout << inputData->BCS[i][0] << ", " << inputData->BCS[i][1] << ", " << inputData->BCS[i][2] << endl;
    }
    std::cout << std::endl;

    std::cout << "\tHBS: " << inputData->XS.size() << endl;
    for (int i = 0; i < inputData->HBS.size(); i++)
    {
        std::cout << "\t  Side " << i + 1 << ": ";
        std::cout << inputData->HBS[i][0] << ", " << inputData->HBS[i][1] << ", " << inputData->HBS[i][2] << endl;
    }
}