// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Railfence class. Here, the key is set, encryption and decryption are done, and the matrix
//     is written to an outside file
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>

#include "Railfence.h"


// Function Prototypes
void GenerateMatrixEnc(std::vector<std::vector<char>>& matrixTmp, int& rowSize, int& col, const std::string& plaintext);
void GenerateMatrixDec(std::vector<std::vector<char>>& matrixTmp, int& rowSize, int& col, const std::string& ciphertext);

// == Class Functions ===============================================================================================================================


// == SetKey ==========================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ====================================================================================================================

bool Railfence::setKey(const string& key)
{
    // If key is larger than 9, check if elements are not digits or wh
    for (int i = 0; i < key.size(); ++i)
    {
        if (isspace(key[i]) || !isdigit(key[i]))
        {
            std::cout << "Invalid Key\n";
            exit(0);
        }
    }

    // Checks key is not 0
    CipherKey = std::stoi(key);
    if (CipherKey <= 0)
    {
        std::cout << "Invalid Key\n";
        exit(0);
    }

    return true;
}
// == end of SetKey ===================================================================================================





// == Encrypt =========================================================================================================
// Input: plaintext string
// Output: ciphertext string
// ====================================================================================================================

std::string Railfence::encrypt(const string& plaintext)
{
    // Variables
    int col = 0;
    int rowSize = CipherKey - 1;
    std::vector<std::vector<char>> matrix;

    // Generates Matrix
    GenerateMatrixEnc(matrix, rowSize, col, plaintext);

    // Prints Matrix
    PrintMatrix(matrix, CipherKey, col + 1);

    // CiperText is assigned to the content of the matrix (read from left to right)
    for (int i = 0; i < CipherKey; ++i)
        for (int j = 0; j < col + 1; ++j)
        {
            if (matrix[i][j] != '\0')
                Ciphertext.push_back(matrix[i][j]);
        }

    return Ciphertext;
}
// == end of Encrypt ==================================================================================================





// == Decrypt =========================================================================================================
// Input: ciphertext string
// Output: plaintext string
// ====================================================================================================================

std::string Railfence::decrypt(const string& ciphertext)
{
    // Variables
    int row = 0;
    int col = 0;
    int rowSize = CipherKey - 1;
    int colSize = 0;
    std::vector<std::vector<char>> matrix;

    // Generates Matrix
    GenerateMatrixDec(matrix, rowSize, colSize, ciphertext);
    
    // Prints matrix to a file
    PrintMatrix(matrix, CipherKey, colSize + 1);

    // Gets plaintext by reading from top to bottom every column
    row = 0;
    for (col = 0; col < colSize + 1;) 
    {
        if (row == CipherKey)
        {   
            row = 0;
            col++;
            continue;
        }
        if (matrix[row][col] != '\0')
        {
            Plaintext.push_back(matrix[row][col]);
            ++row;
        }
        else
            break;
    }

    return Plaintext;
}
// == end of Decrypt ==================================================================================================





// == PrintMatrix =====================================================================================================
// Input: cipher matrix and its row and column sizes
// Output: matrix written to an putside file
// ====================================================================================================================

void Railfence::PrintMatrix(std::vector<std::vector<char>> matrixTmp, const int& rowSize, const int& colSize)
{
    ofstream outFile;
	outFile.open("Matrix.txt");

	outFile << "Matrix Table for the Railfence Cipher\n\n";
	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = 0; j < colSize; ++j)
		{
			if (matrixTmp[i][j] != '\0')
                outFile << setw(5) << matrixTmp[i][j] << " ";
		}	
		outFile << "\n\n" << std::endl;	
	}
}
// == end of PrintMatrix ==============================================================================================

// == end of Class Functions ========================================================================================================================





// == GenerateMatrixEnc =============================================================================================================================
// Input: reference to a matrix, row and column sizes, and plaintext string
// Output: cipher matrix with its content
// ==================================================================================================================================================

void GenerateMatrixEnc(std::vector<std::vector<char>>& matrixTmp, int& rowSize, int& col, const std::string& plaintext)
{
    // Variables
    int row = 0;
    int counter = 0;
    int CipherKey = 1 + rowSize;
    int colSize = plaintext.size()/CipherKey;
    int sizeMatrix = 0;
    
    if (plaintext.size() % CipherKey != 0)
        colSize += 1;

    // Resize the matrix according to rows and columns
    matrixTmp.resize(CipherKey, std::vector<char>(colSize, '\0'));

    // Fills up the matrix with plaintext context 
    for (int i = 0; i < plaintext.size(); ++i)
    {
        if (row == CipherKey)
        {
            row = 0;
            ++col;
        }
        matrixTmp[row][col] = plaintext[i];
        ++row;
    }
    
    // Calculations to get index(ces) with junk
    sizeMatrix = CipherKey * (col + 1);
    sizeMatrix = sizeMatrix - plaintext.size();

    if (sizeMatrix > 0)
        // Removes junk for indecated index(ces)
        while (counter < sizeMatrix)
        {
            matrixTmp[rowSize][col] = '\0';
            ++counter;
            --rowSize;
        }
}
// == end of GenerateMatrixEnc ======================================================================================================================





// == GenerateMatrixDec =============================================================================================================================
// Input: reference to a matrix, row and col sizes, and ciphertext string
// Output: cipher matrix with its content
// ==================================================================================================================================================

void GenerateMatrixDec(std::vector<std::vector<char>>& matrixTmp, int& rowSize, int& col, const std::string& ciphertext)
{
    // Variables
    int row = 0;
    int sizeMatrix = 0;
    int counter = 0;
    int CipherKey = 1 + rowSize;
    int colSize = ciphertext.size()/CipherKey;
    int result = ciphertext.size() % CipherKey;
    int flag = result;
    
    if (result != 0)
    {
        // add a letter to the rows
        colSize += 1;

        // Resize matrix according to the rows (depts of CipherKey) and columns
        matrixTmp.resize(CipherKey, std::vector<char>(colSize, '\0'));

        // Fill up the matrix with ciphertext str
        for (int i = 0; i < ciphertext.size(); ++i)
        {
            // Reset columns and start new row, flag represents extra elements in the matrix
            if (col > colSize - 1)
            {
                col = 0;
                ++row;
                --i;
                --flag;
                continue;
            }
            // If flag is 0, then start a consistent number of columns
            if (flag == 0)
                {
                    colSize = (ciphertext.size() / CipherKey);
                }
            // Fill up the matrix
            matrixTmp[row][col] = ciphertext[i];
            ++col;
        }

        // Calculations to get the amount of junk in matrix
        colSize = col + 1;
        sizeMatrix = CipherKey * (col + 1);
        sizeMatrix = sizeMatrix - ciphertext.size();

        // Removes junks and replace with '\0'
        while (counter < sizeMatrix)
        {
            matrixTmp[rowSize][col] = '\0';
            ++counter;
            --rowSize;
        }     
    }
    // If the matrix is full
    else
    {
        colSize = ciphertext.size() / CipherKey;

        // Resize matrix according to the rows (depts of CipherKey) and columns
        matrixTmp.resize(CipherKey, std::vector<char>(colSize, '\0'));
        
        // Fill up matrix with ciphertext
        for (int i = 0; i < ciphertext.size(); ++i)
        {
            if (col == colSize)
            {
                col = 0;
                ++row;
            }
            matrixTmp[row][col] = ciphertext[i];
            ++col;
        }
    }
}
// == end of GenerateMatrixDec ======================================================================================================================