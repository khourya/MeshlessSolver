#include "Structures.h"

// 2D double
void LogMat(const char* c, std::vector<std::vector<double>> A)
{
    int I = static_cast<int>(A.size());
    if (I == 0)
    {
        std::cout << c << " is an empty matrix." << std::endl;
        return;
    }

    std::cout << c << ": ";
    for (int i = 0; i < I; i++)
    {
        int J = static_cast<int>(A[i].size());
        std::cout << "\t[";
        for (int j = 0; j < J; j++)
        {
            if (j == J - 1)
                std::cout << A[i][j];
            else
                std::cout << A[i][j] << ",\t";
        }
        std::cout << "\t]\n";
    }
    // std::cout << c << "[" << i << "][" << j << "]: " << val << std::endl;
}

// 2D int
void LogMat(const char* c, std::vector<std::vector<int>> A)
{
    int I = static_cast<int>(A.size());
    if (I == 0)
    {
        std::cout << c << " is an empty matrix." << std::endl;
        return;
    }

    std::cout << c << ": ";
    for (int i = 0; i < I; i++)
    {
        int J = static_cast<int>(A[i].size());
        std::cout << "\t[";
        for (int j = 0; j < J; j++)
        {
            if (j == J - 1)
                std::cout << A[i][j];
            else
                std::cout << A[i][j] << ",\t";
        }
        std::cout << "\t]\n";
    }
    // std::cout << c << "[" << i << "][" << j << "]: " << val << std::endl;
}

// 1D double
void LogMat(const char* c, std::vector<double> A)
{
    int I = static_cast<int>(A.size());
    if (I == 0)
    {
        std::cout << c << " is an empty matrix." << std::endl;
        return;
    }

    std::cout << c << ":\t[";
    for (int i = 0; i < I; i++)
    {
        if (i == 0)
            std::cout << A[i] << ",\n";
        else if (i == I - 1)
            std::cout << "\t " << A[i] << "]\n";
        else
            std::cout << "\t " << A[i] << ",\n";
    }
    // std::cout << c << "[" << i << "][" << j << "]: " << val << std::endl;
}

// 1D int
void LogMat(const char* c, std::vector<int> A)
{
    int I = static_cast<int>(A.size());
    if (I == 0)
    {
        std::cout << c << " is an empty matrix." << std::endl;
        return;
    }

    std::cout << c << ":\t[";
    for (int i = 0; i < I; i++)
    {
        if (i == 0)
            std::cout << A[i] << ",\n";
        else if (i == I - 1)
            std::cout << "\t " << A[i] << "]\n";
        else
            std::cout << "\t " << A[i] << ",\n";
    }
    // std::cout << c << "[" << i << "][" << j << "]: " << val << std::endl;
}