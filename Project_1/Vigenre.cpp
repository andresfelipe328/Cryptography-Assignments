// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Vigenre class. Here, the key is set, encryption and decryption are done.
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

#include "Vigenre.h"


// == Class Functions ===============================================================================================================================


// == SetKey ==========================================================================================================
// Input: cipher key
// Output: boolean value determining validity of the key 
// ====================================================================================================================

bool Vigenre::setKey(const string& key)
{
    // Checks if all characters are alphabetic
	if (std::find_if(key.begin(), key.end(), std::not1(std::ptr_fun((int(*)(int))std::isalpha))) != key.end())
	{
        cout << "Invalid Key\n";
		exit(0);
	}
    
    CipherKey = key;
	// Turns key into upper case
	std::for_each(CipherKey.begin(), CipherKey.end(), [](char & c) {c = ::toupper(c);});

    return true;
}
// == end of SetKey ===================================================================================================





// == Encrypt =========================================================================================================
// Input: plaintext string
// Output: ciphertext string 
// ====================================================================================================================

std::string Vigenre::encrypt(const string& plaintext)
{
    // Variables
    char ch;

    // Fills up the key by combining the key and plaintext
    for (int i = 0; i < plaintext.size(); ++i)
    {
        CipherKey.push_back(plaintext[i]);
        if ( CipherKey.size() == plaintext.size())
            break;
    }

    // Matches plaintext and cipher key to get encrypted letter accorging to Vignere's matrix
    for (int i = 0; i < plaintext.size(); ++i)
    {
        ch = (plaintext[i] + CipherKey[i]) % 26;
        ch += 'A';
        Ciphertext.push_back(ch);
    }
    
    return Ciphertext;
}
// == end of Encrypt ==================================================================================================





// == Decrypt =========================================================================================================
// Input: ciphertext string
// Output: plaintext string
// ====================================================================================================================

std::string Vigenre::decrypt(const string& ciphertext)
{
    // Variables
    char ch;

    // Matches ciphertext and cipher key to get decrypted letter, then letter is added to key to contiue descryption
    for (int i = 0; i < ciphertext.size(); ++i)
    {
        ch = ((ciphertext[i] - CipherKey[i]) + 26) % 26;
        ch += 'A';
        Plaintext.push_back(ch);
        CipherKey.push_back(ch);
    }

    // Turns plaintext into lower case
	std::for_each(Plaintext.begin(), Plaintext.end(), [](char & c) {c = ::tolower(c);});
    
    return Plaintext;
}
// == end of Decrypt ==================================================================================================

// == End of Class Functions ========================================================================================================================