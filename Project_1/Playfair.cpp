// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Playfair class. Here, the key is set, encryption and decryption are done, and the matrix
//     is written to an outside file
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <functional>

#include "Playfair.h"

// Function Prototypes
std::string MakeEvenNonRepetitive(std::string text);
void GenerateMatrix(char (&matrix)[5][5], const std::string& key);
void SearchLocation(char matrix[5][5],  char a, char b, int* coordinates);

// == Class Functions ===============================================================================================================================


// == SetKey ==========================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ====================================================================================================================

bool Playfair::setKey(const std::string& key)
{ 
	// Variable
	int j;
	std::string tmp;

	// Checks if all characters are alphabetic
	if (std::find_if(key.begin(), key.end(), std::not1(std::ptr_fun((int(*)(int))std::isalpha))) != key.end())
	{
        cout << "Invalid Key\n";
		exit(0);
	}

	// Assing key to a temp for lower case conversion
	tmp = key;

	// Turns plaintext into lower case
	std::for_each(tmp.begin(), tmp.end(), [](char & c) {c = ::tolower(c);});

	// Iterates through the key to ignore duplicate letters
	for (int i = 0; i < key.size(); ++i)
	{
		// If after first occurance, there is duplicate, ignore elements and continue 
		for (j = 0; j < i; ++j)
			if (tmp[i] == tmp[j])
				break;
		// If inner for loop reaches value of i, then element is unique and is added to Cipherkey
		if (j == i)
			CipherKey.push_back(tmp[i]);
	}

	return true;
}
// == end of SetKey ===================================================================================================





// == Encrypt =========================================================================================================
// Input: plaintex string
// Output: ciphertext string
// ====================================================================================================================

std::string Playfair::encrypt(const std::string& plaintext)
{ 
	// Variable
	std::string ciphertext;
	char matrix[5][5];
	int coordinates[4];

	// Adds an extra element to plaintext to make it even
	ciphertext = MakeEvenNonRepetitive(plaintext);

	// Generates matrix for encrypting
	GenerateMatrix(matrix, CipherKey);
	SetMatrix(matrix);
	PrintMatrix();

	// Encyption by searching location on matrix and switching letters
	for (int i = 0; i < ciphertext.size(); i += 2)
	{
		// returns coordinates of a pair ([x1],[x2],[y1],[y2])
		SearchLocation(matrix, ciphertext[i], ciphertext[i + 1],coordinates);

		// If a pair has the same x1 and y1 (same row, select one the right, % to ensure wrap around)
		if (coordinates[0] == coordinates[2]) 
		{
            ciphertext[i] = matrix[coordinates[0]][(coordinates[1] + 1) % 5];
            ciphertext[i + 1] = matrix[coordinates[0]][(coordinates[3] + 1) % 5];
        }
		// else if pair has the same x2, y2 (same column, select one from below, % to ensure wrap around)
        else if (coordinates[1] == coordinates[3]) 
		{
            ciphertext[i] = matrix[(coordinates[0] + 1) % 5][coordinates[1]];
            ciphertext[i + 1] = matrix[(coordinates[2] + 1) % 5][coordinates[1]];
        }
		// else, pair land in different row-column combination
        else 
		{
            ciphertext[i] = matrix[coordinates[0]][coordinates[3]];
            ciphertext[i + 1] = matrix[coordinates[2]][coordinates[1]];
        }
	}

	return ciphertext; 
}
// == end of Encrypt ==================================================================================================





// == Decrypt =========================================================================================================
// Input: ciphertext string
// Output: plaintext string
// ====================================================================================================================

std::string Playfair::decrypt(const std::string& cipherText) 
{ 
	// Variable
	std::string plaintext;
	char matrix[5][5];
	int coordinates[4];

	// Adds an extra element to plaintext to make it even
	plaintext = MakeEvenNonRepetitive(cipherText);
	
	// Generates matrix for encrypting
	GenerateMatrix(matrix, CipherKey);
	SetMatrix(matrix);
	PrintMatrix();

	// Decryption by searching location on matrix and switching letters
	for (int i = 0; i < plaintext.size(); i += 2)
	{
		// returns coordinates of a pair ([x1],[x2],[y1],[y2])
		SearchLocation(matrix, plaintext[i], plaintext[i + 1],coordinates);
		// If a pair has the same x1 and y1 (same row, select one the left, % to ensure wrap around)
		if (coordinates[0] == coordinates[2]) 
		{
            plaintext[i] = matrix[coordinates[0]][(coordinates[1] - 1) % 5];
            plaintext[i + 1] = matrix[ coordinates[0] ][ (coordinates[3] - 1) % 5 ];
			// In case of wrap around, -1 is equivalent to 4
			if ((coordinates[1] - 1) % 5 == -1)
				plaintext[i] = matrix[coordinates[0]][4];
			else if ((coordinates[3] - 1) % 5 == -1)
				plaintext[i + 1] = matrix[ coordinates[0] ][4];
        }
		// else if pair has the same x2, y2 (same column, select one from above, % to ensure wrap around)
        else if (coordinates[1] == coordinates[3]) 
		{
            plaintext[i] = matrix[(coordinates[0] - 1) % 5][coordinates[1]];
            plaintext[i + 1] = matrix[(coordinates[2] - 1) % 5][coordinates[1]];
			// In case of wrap around, -1 is equivalent to 4
			if ((coordinates[0] - 1) % 5 == -1)
				plaintext[i] = matrix[4][coordinates[1]];
			else if ((coordinates[2] - 1) % 5 == -1)
				plaintext[i+1] = matrix[4][coordinates[1]];
        }
		// else, pair land in different row-column combination
        else 
		{
            plaintext[i] = matrix[coordinates[0]][coordinates[3]];
            plaintext[i + 1] = matrix[coordinates[2]][coordinates[1]];
        }
	}

	return plaintext; 
}
// == end of Decrypt ==================================================================================================





// == SetMatrix =======================================================================================================
// Input: a reference to a 5 by 5 matrix
// Output: the class matrix that is assinged to the input matrix
// ====================================================================================================================

void Playfair::SetMatrix(const char (&matrixTemp)[5][5])
{
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			matrix[i][j] = matrixTemp[i][j];
}

// == end of SetMatrix ================================================================================================





// == PrintMatrix =====================================================================================================
// Input: none
// Output: matrix written to an outside file 
// ====================================================================================================================

void Playfair::PrintMatrix()
{
	ofstream outFile;
	outFile.open("Matrix.txt");

	outFile << "Matrix Table for the Playfair Cipher\n\n";
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
			outFile << setw(5) << matrix[i][j] << " ";
		outFile << "\n\n" << std::endl;	
	}
}
// == end of PrintMatrix ==============================================================================================

// == end of Class Functions ========================================================================================================================





// == MakeEvenNonRepetitive =========================================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ==================================================================================================================================================

std::string MakeEvenNonRepetitive(std::string text)
{
	// If size is odd, add an x
	if (text.size() % 2 != 0)
		text.push_back('x');

	// If a pair has repeated letters, switch the last one with an x
	for (int i = 0; i <text.size(); i += 2)
		if (text[i] == text[i + 1])
			text[i + 1] = 'x';

	return text;
}
// == end of MakeEvenNonRepetitve ===================================================================================================================





// == GenerateMatrix ================================================================================================================================
// Input: a reference to a matrix and the cipher key
// Output: the matrix with its content
// ==================================================================================================================================================

void GenerateMatrix(char (&matrix)[5][5], const std::string& key)
{
	// Variables
	char matrixContent[26];
	int flag =  0;
	int j = 0;
	int row = 0;
	int col = 0;
	ofstream outFile;

	// Adds key to the matrix content
	for (int i = 0; i <key.size(); ++i)
		matrixContent[i] = key[i];

	// Iterates through the key to decide which letter are duplicates, j is not included becuse j == i
	for (int ch = 'a'; ch <= 'z'; ++ch)
	{
		for (int i = 0; i < key.size(); ++i)
		{
			if (ch == int(key[i]) || ch == 'j')
				flag = 1;
		}
		if (flag != 1)
		{
			matrixContent[(key.size()) + j] = ch;
			++j;
		}
		else
			flag = 0;
	}

	// Creates matrix, every 5 columns in a row, a new row is created
	for (int i = 0; i < 25; ++i)
	{
		if (col != 5)
		{
			matrix[row][col] = matrixContent[i];
			++col;
		}
		else
		{
			++row;
			--i;
			col = 0;
		}
	}
}
// == end of GenerateMatrix =========================================================================================================================





// == SearchLocation ================================================================================================================================
// Input: the cipher's matrix, one pair of elements, and an int array
// Output: The coordinates of the pair of elements in the int array
// ==================================================================================================================================================

void SearchLocation(char matrix[5][5], char a, char b, int* coordinates)
{
	// Cases to ignore j
	if (a == 'j')
		a == 'i';
	else if (b == 'j')
		b == 'j';
	
	// Iterates through the matrix
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			// Saves row and column
			if (matrix[i][j] == a)
			{
				coordinates[0] = i;
				coordinates[1] = j;
			}
			// Saves row and column
			else if (matrix[i][j] == b)
			{
				coordinates[2] = i;
				coordinates[3] = j;
			}
		}
	}
}