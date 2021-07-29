// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Row Transposition class. Here, the key is set, encryption and decryption are done, and the 
//     matrix is written to an outside file
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <functional>

#include "Row.h"

// Function Prototypes
void GenerateMatrixEnc(std::vector<std::vector<char>>& matrixTmp, const int& colSize, int& rowSize, const std::string& plaintext);
void GenerateMatrixDec(std::vector<std::vector<char>>& matrix, const int& colSize, const int& rowSize, const std::string& ciphertext, const int* array);

// == Class Functions ===============================================================================================================================


// == SetKey ==========================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ====================================================================================================================

bool RowTrans::setKey(const std::string& key)
{
    // Variable
    int tmpArray[key.size()];

    // Checks if all characters are numeric
	if (std::find_if(key.begin(), key.end(), std::not1(std::ptr_fun((int(*)(int))std::isdigit))) != key.end())
	{
        cout << "Invalid Key\n";
		exit(0);
	}

    // Iterates through the array to change elements to int
    for (int i = 0; i < key.size(); ++i)
    {
        tmpArray[i] = key[i] - 48;
    }

    // Sort array to see if elements have a difference of 1
    sort(tmpArray, tmpArray + key.size());
    for (int i = 0; i < key.size(); ++i)
        if (i > 0 && (tmpArray[i] - tmpArray[i - 1]) > 1 || (tmpArray[i] - tmpArray[i - 1]) == 0)
        {
            std::cout << "Invalid Key\n";
            exit(0);
        }

    SetupKey(key.size(), tmpArray);
    
    return true;
}
// == end of SetKey ===================================================================================================





// == SetupKey ========================================================================================================
// Input: the size of the cipher key and an int array
// Output: class cipherkey 
// ====================================================================================================================

void RowTrans::SetupKey(const int& size,int* array)
{
    CipherKey = new int[size];
    KeySize = size;
    for (int i = 0; i < size; ++i)
    {
        CipherKey[i] = array[i];
    }

}

// == end of SetupKey =================================================================================================





// == Encrypt =========================================================================================================
// Input: plaintext string
// Output: ciphertext string
// ====================================================================================================================

std::string RowTrans::encrypt(const std::string& plaintext)
{
    // Variables
    int row = 0;
    int col = 0;
    int colSize = KeySize;
    int rowSize = plaintext.size()/KeySize;
    int maxRow = 0;
    std::vector<std::vector<char>> matrix;

    // Generates Matrix
    GenerateMatrixEnc(matrix, colSize, rowSize, plaintext);
    // Prints matrix to its file
    PrintMatrix(matrix, rowSize, colSize);

    // Gets the ciphertext by reading columns (top to bottom) accoring to the key
    for (int i = 0; i < KeySize; ++i)
    {
        if (row == rowSize)
            row = maxRow = 0;
        Ciphertext.push_back(matrix[row][CipherKey[i] - 1]);
        ++row;
        ++maxRow;
        if (maxRow < rowSize)
            --i;
    }

    return Ciphertext;
}
// == end of Encrypt ==================================================================================================





// == Decrypt =========================================================================================================
// Input: ciphertext string
// Output: plaintext string
// ====================================================================================================================

std::string RowTrans::decrypt(const std::string& ciphertext)
{
    int row = 0;
    int col = 0;
    int colSize = KeySize;
    int rowSize = (ciphertext.size()/KeySize);
    int maxRow = 0;
    std::vector<std::vector<char>> matrix;

    // Generates Matrix
    GenerateMatrixDec(matrix, colSize, rowSize, ciphertext, CipherKey);
    // Prints matrix to its file
    PrintMatrix(matrix, rowSize, colSize);

    for (int i = 0; i < rowSize; ++i)
            for (int j = 0; j < colSize; ++j)
                Plaintext.push_back(matrix[i][j]);

    return Plaintext;
}
// == end of Decrypt ==================================================================================================





// == PrintMatrix =====================================================================================================
// Input: the cipher matrix with its row and column size
// Output: the matrix written to an outside file 
// ====================================================================================================================

void RowTrans::PrintMatrix(std::vector<std::vector<char>> matrixTmp, const int& rowSize, const int& colSize)
{
    ofstream outFile;
	outFile.open("Matrix.txt");

	outFile << "Matrix Table for the Row Transposition Cipher\n\n";
	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = 0; j < colSize; ++j)
		{
			outFile << setw(5) << matrixTmp[i][j] << " ";
		}	
		outFile << "\n\n" << std::endl;	
	}
}
// == end of PrintMatrix ==============================================================================================

// == end of Class Functions ========================================================================================================================





// == GenerateMatrixEnc =============================================================================================================================
// Input: reference to a matrix, row and column sizes, and plaintext string
// Output: the matrix with its content 
// ==================================================================================================================================================

void GenerateMatrixEnc(std::vector<std::vector<char>>& matrix, const int& colSize, int& rowSize, const std::string& plaintext)
{
    // Variables
    int row = 0;
    int col = 0;
    bool flag = false;

    if ((rowSize * colSize) != plaintext.size())
    {
        rowSize = rowSize + 1;
        flag = true;
    }
    // Resizes matrix according to rowSize and colSize
    matrix.resize(rowSize, std::vector<char>(colSize, '\0'));

    // Fills up the matrix by going row by row, top to bottom
    for (int i = 0; i < plaintext.size(); ++i)
    {
        if (col == colSize)
        {
            col = 0;
            ++row;
        }
        matrix[row][col] = plaintext[i];
        ++col;
    }

    // If matrix is  not full, fill it up with letters
    if (flag)                        
    {
        // Variables
        int sizeMatrix = 0;
        int counter = 0;
        int maxRowSize = rowSize - 1;
        int maxColSize = colSize - 1;
        char ch;

        // Determines how many elements have junk
        sizeMatrix = (row + 1) * colSize;
        sizeMatrix = sizeMatrix - plaintext.size();

        // Letters are assigned to the empty elements
        while (counter < sizeMatrix)
        {
            if (counter == 0)
                ch = 'z';

            matrix[maxRowSize][maxColSize] = ch;
            ++counter;
            --maxColSize;
            --ch;
        }
    }

}
// == end of GenerateMatrixEnc ======================================================================================================================





// == GenerateMatrixDec =============================================================================================================================
// Input: reference to a matrix, row and column sizes, ciphertext string, and cipher key (int array)
// Output: the matrix with its content 
// ====================================================================================================================

void GenerateMatrixDec(std::vector<std::vector<char>>& matrix, const int& colSize, const int& rowSize, const std::string& ciphertext, const int* array)
{
    // Variables
    int row = 0;
    int col = 0;
    int index = 0;
    int isFull = 0;
    int maxRow = 0;

    // Resizes matrix according to rowSize and colSize
    matrix.resize(rowSize, std::vector<char>(colSize, '\0'));

    // Matrix is made up accoring to the indeces (columns) of the key, elemenets are read from top to bottom
    for (int i = 0; i < colSize; ++i)
    {
        if (row == rowSize)
            row = maxRow = 0;
        matrix[row][array[i] - 1] = ciphertext[index];
        ++row;
        ++maxRow;
        ++index;
        if (maxRow < rowSize)
            --i;
    }

}
// == end of GenerateMatrixDec ======================================================================================================================