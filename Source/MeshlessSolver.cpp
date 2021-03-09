// MeshlessSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MeshlessSolver.h"

int main()
{
    // Boolean for checking stuff
    bool checker = false;
    bool runUnitTests = true;
    bool runCode = false;

    // Intro Message ----------------------------------------------------------------------------------
    std::cout << "Localized RBF Collocation Meshless Method" << std::endl;
    std::cout << "Mechanical Engineering Department" << std::endl;
    std::cout << "Embry-Riddle Aeronautical University (ERAU)" << "\n\n";
    // ------------------------------------------------------------------------------------------------

    if (runUnitTests)
    {
    // Running Unit Tests
    std::cout << "Running Unit Tests........................................." << std::endl;
    bool passedAllTests = RunUnitTests();
    std::cout << " done!" << std::endl;

    if (!passedAllTests)
        return -1;
    // ------------------------------------------------------------------------------------------------
    }

    // Flag for Error Reporting
    int errorFlag = 0;

    // Data Structures to be used
    Options options;
    InputData inputData;
    PreprocessorData preProcData;
    SolutionData solution;

    if (runCode)
    {
    // Reading Options --------------------------------------------------------------------------------
    std::cout << "Reading options file.......................................";
    errorFlag = ReadOptions(&checker, &options);
    if (errorFlag != 0)
    {
        LogErrors(errorFlag);
        return -1;
    }
    std::cout << " done!" << std::endl;
    // ------------------------------------------------------------------------------------------------

    // Reading Input ----------------------------------------------------------------------------------
    std::cout << "Reading input file.........................................";
    errorFlag = ReadInputs(&checker, &inputData, &preProcData);
    if (errorFlag != 0)
    {
        LogErrors(errorFlag);
        return -1;
    }
    std::cout << " done!" << std::endl;
    // ------------------------------------------------------------------------------------------------

    // Preprocessing ----------------------------------------------------------------------------------
    std::cout << "Generating geometric and field data........................";
    errorFlag = PreprocessDomain(&checker, &options, &inputData, &preProcData, &solution);
    if (errorFlag != 0)
    {
        LogErrors(errorFlag);
        return -1;
    }
    std::cout << " done!" << std::endl;
    // ------------------------------------------------------------------------------------------------

    // Perform Collocation ----------------------------------------------------------------------------
       // Builds local topologies if the strategy requires it
    std::cout << "Applying collocation methodology..........................." << std::endl;
    CollocationData collocationData;
    InterpolationVectors interpolationVectors;
    errorFlag = Collocation(&checker, &options, &preProcData, &collocationData, &interpolationVectors);
    if (errorFlag != 0)
    {
        LogErrors(errorFlag);
        return -1;
    }
    std::cout << "Completed collocation methodology, done!" << std::endl;
    // ------------------------------------------------------------------------------------------------

    // Computing Transient Solution -------------------------------------------------------------------
    std::cout << "Calculating solution.......................................\n";
    errorFlag = Solver(&checker, &inputData, &preProcData, &collocationData, &interpolationVectors, &solution);
    if (errorFlag != 0)
    {
        LogErrors(errorFlag);
        return -1;
    }
    std::cout << " done!" << std::endl;
    // ------------------------------------------------------------------------------------------------
    }
}


/*
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   GENERATE PROBLEM DATA
CCC
      WRITE (*,*) 'GENERATING GEOMETRIC AND FIELD DATA................'
      CALL PREPROCESSOR
      WRITE (*,*) '...............................................DONE'
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   BUILD INFLUENCE TOPOLOGIES
CCC
      WRITE (*,*) 'BUILDING INFLUENCE TOPOLOGIES......................'
      CALL TOPOLOGY
      WRITE (*,*) '...............................................DONE'
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   PERFORM COLLOCATION
CCC
      WRITE (*,*) 'PERFORMING COLLOCATION.............................'
      CALL COLLOCATION
      WRITE (*,*) '...............................................DONE'
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   OPEN CSV AND TECPLOT OUTPUT FILES AND WRITE HEADERS
CCC
C     CSV's for Tissue and Blood
      OPEN (13,FILE='Data/LRCMM_PK_2D_Tissue.csv')
C
      OPEN (14,FILE='Data/LRCMM_PK_2D_Blood.csv')

C     PLT's for Tissue and Blood
      OPEN (15,FILE='Data/LRCMM_PK_2D_Tissue.plt')
      WRITE (15,'("TITLE = ",A,A120,A)') CHAR(34),TITLE,CHAR(34)
C
      OPEN (16,FILE='Data/LRCMM_PK_2D_Blood.plt')
      WRITE (16,'("TITLE = ",A,A120,A)') CHAR(34),TITLE,CHAR(34)
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   COMPUTE TRANSIENT SOLUTION FIELD
CCC
      WRITE (*,*) 'PERFORMING TRANSIENT SOLUTION......................'
C
C     WRITE INITIAL OUTPUT FILES
C
      N=0
      WRITE (*,*) '  WRITING OUTPUT FILES AT TIME STEP: ',N,' OF: ',NT
      CALL OUTPUT(N)
C
C     TIME STEP SIZE
C
      DT=TF/DBLE(NT)
C
C     LOOP OVER TIME STEPS
C
      DO N=1,NT
C
       CALL SOLUTION(N,DT)
C
C      WRITE OUTPUT FILES IF TIME STEP COINCIDES WITH OUTPUT FREQUENCY
C
       IF (MOD(N,NTOUT).EQ.0) THEN
        WRITE (*,*) '  WRITING OUTPUT FILES AT TIME STEP: ',N,' OF: ',NT
        CALL OUTPUT(N)
       END IF
C
      END DO
C
      WRITE (*,*) '...............................................DONE'
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
CCC   CLOSE TEXT AND TECPLOT OUTPUT FILES
CCC
      CLOSE (13)
      CLOSE (16)
CCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC
      STOP
      END
*/



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
