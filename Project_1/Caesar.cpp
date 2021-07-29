// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Caesar class. Here, the key is set, encryption and decryption are done.
// ==================================================================================================================================================


#include <iostream>
#include <fstream>

#include "Caesar.h"

// == Class Functions ===============================================================================================================================


// == SetKey ==========================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ====================================================================================================================

bool Caesar::setKey(const string& key)
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

std::string Caesar::encrypt(const string& plaintext)
{
    // Variables
    char keyLetter;
    int total = 0;
    char remainder;

    // Iterate through plaintext and discover the shifts based on key
    for (int i = 0; i < plaintext.size(); ++i)
    {
        keyLetter = plaintext[i] + CipherKey;
        total = int(keyLetter);
        
        // When total is greated than 122 (z), wrap around 
        if (total > 122)
        {
            // Gets shifts remainding
            remainder = char(total - 122);
            keyLetter = '`' + remainder;
        }

        Ciphertext.push_back(keyLetter);    
    }
    
    return Ciphertext;
}
// == end of Encrypt ==================================================================================================





// == Decrypt =========================================================================================================
// Input: ciphertext string
// Output: plaintext string
// ====================================================================================================================

std::string Caesar::decrypt(const string& ciphertext)
{
    // Variables
    char keyLetter;
    int total = 0;
    char remainder;

    // Iterate through plaintext and discover the shifts based on key
    for (int i = 0; i < ciphertext.size(); ++i)
    {
        keyLetter = ciphertext[i] - CipherKey;
        total = int(keyLetter);

        // When total is greated than 122 (z), wrap around 
        if (total < 97)
        {
            // Gets shifts remainding
            remainder = char(97 - total);
            keyLetter = '{' - remainder;
        }

        Plaintext.push_back(keyLetter);    
    }
    
    return Plaintext;
}
// == end of Decrypt ==================================================================================================

// == end of Class Functions ========================================================================================================================