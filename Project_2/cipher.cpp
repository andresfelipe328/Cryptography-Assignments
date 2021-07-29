// ==================================================================================================================================================
// Author: Andres F. Romero N
// Date Finished: 04/30/21
// File Name: cipher.cpp
// Language: C++
// Description: This file contains the main function of the program. This is where in/output files are open to read and write cipher/plaintext. In
//		addition, cipher keys are configured to be used by AES/DES implementation files. To fulfill the ciphers' block requirements, padding is added
//		to blocks of text that are less than the expected. The amount of padding is represented by a number placed at the end of the plaintext. 
//
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <functional>

// Cipher classes
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;


// Function Prototypes
void 	CheckAndConvertDESKey(string keyStr, unsigned char* key);
void 	CheckAndConvertAESKey(string keyStr, unsigned char* key, string method);
void 	OpenReadnWrite(CipherInterface* cipher, string cipherType, string method, string inputFile, string outputFile);
void 	PaddMessage(unsigned char* message, int read, const int bufferSize);

int main(int argc, char** argv)
{
	// Variables
	int				padding = 8;
	string 			cipherType = argv[1];
	string 			keyStr = argv[2];
	string 			method = argv[3];
	string 			inputFile = argv[4];
	string 			outputFile = argv[5];
	string 			message;
	int 			bufferSize = 0;
	
	// If it's DES cipher
	if (cipherType == "DES")
	{
		// Converts key to unsigned char array
		int keySize = 16;
		unsigned char 	key[keySize];
		CheckAndConvertDESKey(keyStr, key);
		bufferSize = 8;

		/* Create an instance of the DES cipher */	
		CipherInterface* cipher = new DES();

		// Sets the key for cipher 
		if (!cipher->setKey(key))
		{
			cout << "Invalid Key\n";
			exit(1);
		}
		if (method == "ENC")
		{
			// Perform encryption
			OpenReadnWrite(cipher, cipherType, method, inputFile, outputFile);
		}
		else if (method == "DEC")
		{
			// Perform decryption 
			OpenReadnWrite(cipher, cipherType, method, inputFile, outputFile);
		}
	}
	else if (cipherType == "AES")
	{
		// Converts key to unsigned char array
		int keySize = 33;
		unsigned char key[keySize];
		CheckAndConvertAESKey(keyStr, key, method);
		bufferSize = 16;

		/* Create an instance of the DES cipher */	
		CipherInterface* cipher = new AES();

		// Sets the key for cipher 
		if (!cipher->setKey(key))
		{
			cout << "Invalid Key\n";
			exit(1);
		}
		if (method == "ENC")
		{
			// Perform encryption
			OpenReadnWrite(cipher, cipherType, method, inputFile, outputFile);
		}
		else if (method == "DEC")
		{
			// Perform decryption 
			OpenReadnWrite(cipher, cipherType, method, inputFile, outputFile);
		}
	}
	return 0;
}





// == CheckAndConvertAESKey =========================================================================================================================

void CheckAndConvertDESKey(string keyStr, unsigned char* key)
{
	// Variable
	int size = 16;
	
	// If Key is larger than 16 characters, it's an invalid key
	if (keyStr.size() > size)
	{
		cout << "Invalid Key\n";
		exit(1);
	}
	
	// Assings the string to unsinged char array
    for (int i = 0; i < size; ++i)
    {
    	key[i] = keyStr[i];
	}
}

// == end of CheckAndConvertDESKey ==================================================================================================================





// == CheckAndConvertAESKey =========================================================================================================================

void CheckAndConvertAESKey(string keyStr, unsigned char* key, string method)
{
	// Variable
	int size = 32;

	// If Key is larger than 32 characters, it's an invalid key
	if (keyStr.size() > size)
	{
		cout << "Invalid Key\n";
		exit(1);
	}
	for (int i = 0; i < size; ++i)
    	{
    		key[i] = keyStr[i];
		}
}

// == end of CheckAndConvertAESKey ==================================================================================================================





// == FileConversion ================================================================================================================================
void OpenReadnWrite(CipherInterface* cipher, string cipherType, string method, string inputFile, string outputFile)
{
	// Variables
	const char* inFileName = inputFile.c_str();
	const char* outFileName = outputFile.c_str();
	FILE *fpIn = fopen(inFileName, "rb");
	FILE *fpOut = fopen(outFileName, "wb");

	// Getting File Size
	ifstream in_file(inFileName, ios::binary);
   	in_file.seekg(0, ios::end);
   	int file_size = in_file.tellg();
	int totalBytes = 0;

	if (!fpIn)
	{
		perror("fopen");
		exit(0);
	}
	while (totalBytes < file_size)
	{
		if (cipherType == "DES")
		{
			// Variables
			unsigned char buffer[9];
			int blockSize = 8;

			// If encrypting in DES
			if (method == "ENC")
			{
				// Read in chunks of 8, pad if needed
				int read = fread(buffer, 1, blockSize, fpIn);
				totalBytes += read;
		
				if (read < blockSize)
					PaddMessage(buffer, read, blockSize);
				else
					buffer[blockSize] = '\0';
				
				// Encrypt and receive block to write to file
				unsigned char *ciphertextBlock = cipher->encrypt(buffer);
				fwrite(ciphertextBlock, 1, blockSize, fpOut);
				memset(buffer, 0, sizeof(unsigned char*));
			}
			// If decrypting in DES
			if (method == "DEC")
			{
				// Read in chunks of 8
				int read = fread(buffer, 1, blockSize, fpIn);
				totalBytes += read;

				// Decrypt and receive block to write to file
				unsigned char *plaintext = cipher->decrypt(buffer);
				fwrite(plaintext, 1, blockSize, fpOut);
				memset(buffer, 0, sizeof(unsigned char*));
			}
		}
		if (cipherType == "AES")
		{
			// Variables
			unsigned char buffer[17];
			int blockSize = 16;

			// If encrypting in AES
			if (method == "ENC")
			{
				// Read in chunks of 16, pad if needed
				int read = fread(buffer, 1, blockSize, fpIn);
				totalBytes += read;
				
				if (read < blockSize)
					PaddMessage(buffer, read, blockSize);
				else
					buffer[blockSize] = '\0';
				
				// Encrypt and receive block to write to file
				unsigned char *ciphertextBlock = cipher->encrypt(buffer);
				fwrite(ciphertextBlock, 1, blockSize, fpOut);
				memset(buffer, 0, sizeof(unsigned char*));
			}
			// If decrypting in AES
			if (method == "DEC")
			{
				// Read in chunks of 16
				int read = fread(buffer, 1, blockSize, fpIn);
				totalBytes += read;
				
				buffer[blockSize] = '\0';

				// Decrypt and receive block to write to file
				unsigned char *plaintext = cipher->decrypt(buffer);
				fwrite(plaintext, 1, blockSize, fpOut);
				memset(buffer, 0, sizeof(unsigned char*));
			}
		}

	}
	fclose(fpIn);
	fclose(fpOut);
}





// == PaddingMessage ================================================================================================================================
void PaddMessage(unsigned char* message, int read, const int bufferSize)
{
	// Variables
	int counter = 0;
	int remainder = bufferSize - read;
	
	// Pad until counter is 8
	while (counter != remainder)
	{
		if (counter == remainder)
		{
			break;
		}
		message[read] = '0';
		++counter;
		++read;
	}
	// Add amount of pad
	if (counter < 10)
		message[read - 1] = '0' + counter;
	else
	{
		message[read - 2] = '0' + 1;
		message[read - 1] = '0' + (counter % 10);
	}
	message[read] = '\0';
}