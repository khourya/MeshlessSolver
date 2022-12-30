// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "ErrorLogger.h"
#include "MeshlessSolver.h"

int main()
{
    std::cout << "Hello World!\n";

    // Flag for Error Reporting
    int errorFlag = 0;
    Logger* logger = new Logger();

    // Boolean for checking stuff
    bool checker = true;
    bool runUnitTests = false;
    bool runCode = true;

    // Reading Options --------------------------------------------------------------------------------
    std::cout << "Reading options file.......................................";
    Options options;
    errorFlag = ReadOptions(&checker, &options);
    if (errorFlag != 0)
    {
        logger->LogErrorsWarnings(errorFlag);
        return -1;
    }
    std::cout << "... done!" << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
