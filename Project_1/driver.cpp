// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This file creates the intances of the different cipher classes and passes the user's arguments to their functions. The output
//     are an encryption file (enc.txt) when encrypting, a decryption file (dec.txt) when decrypting, and a matrix file (if applicable)
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

// Cipher Types
#include "CipherInterface.h"
#include "Playfair.h"
#include "Railfence.h"
#include "Row.h"
#include "Caesar.h"
#include "Vigenre.h"
#include "Hill.h"


// Function Prototypes
std::string FileConversion(std::string file, std::string cipher);

int main(int argc, char** argv)
{
    // Variables
    std::string     cipherType = argv[1];
    std::string     key = argv[2];
    std::string     method = argv[3];
    std::string     inputFile = argv[4];
    std::string     outputFile = argv[5];
    std::string     result;
    ofstream        outFile;
    std::string     message;

    // Validation of input file and to string conversion
    message = FileConversion(inputFile, cipherType);

    // Playfair Cipher
    if (cipherType == "PLF")
    {
        CipherInterface* cipher = new Playfair();

        // Set the encryption key
        cipher->setKey(key);
	
        // Encrypt and write result to output file
	    if (method == "ENC")
        {
	        result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;

        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    // Railfence Cipher
    if (cipherType == "RFC")
    {
        CipherInterface* cipher = new Railfence();

        // Set the encryption key
        cipher->setKey(key);

        // Encrypt and write result to output file
        if (method == "ENC")
        {
            result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    // Row Transpostition Cipher
    if (cipherType == "RTS")
    {
        CipherInterface* cipher = new RowTrans();

        // Set the encryption key
        cipher->setKey(key);

        // Encrypt and write result to output file
        if (method == "ENC")
        {
            result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    // Caesar Cipher
    if (cipherType == "CES")
    {
        CipherInterface* cipher = new Caesar();

        // Set the encryption key
        cipher->setKey(key);

        // Encrypt and write result to output file
        if (method == "ENC")
        {
            result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    // Vigenre Cipher
    if (cipherType == "VIG")
    {
        CipherInterface* cipher = new Vigenre();

        // Set the encryption key
        cipher->setKey(key);

        // Encrypt and write result to output file
        if (method == "ENC")
        {
            result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    // Hill Cipher
    if (cipherType == "HIL")
    {
        CipherInterface* cipher = new Hill();

        // Set the encryption key
        cipher->setKey(key);

        // Encrypt and write result to output file
        if (method == "ENC")
        {
            result = cipher->encrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
        // Decrypt and write result to output file
        else if (method == "DEC")
        {
            result = cipher->decrypt(message);
            outFile.open(outputFile);
            outFile << result;
        }
    }

    return 0;
}





// == FileConversion ================================================================================================================================
// Input: two strings that represent the cipher and its key
// Output: boolean value that determines validity of key
// ==================================================================================================================================================

std::string FileConversion(std::string file, std::string cipher)
{
    // Variables
    ifstream inFile;
    std::string message;

    inFile.open(file);
    if (!inFile.is_open())
    {
        std::cout << "File unable to open\n";
        exit(0);
    }

    // Reads the file and converts it to a string
    message  = string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    
    // converts string to lower case
    if (cipher != "VIG")
        std::for_each(message.begin(), message.end(), [](char & c) {c = ::tolower(c);});
    else
        std::for_each(message.begin(), message.end(), [](char & c) {c = ::toupper(c);});

    return message;
}
// == end of FileConversio ==========================================================================================================================