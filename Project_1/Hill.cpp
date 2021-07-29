// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the implementation of the Hill class. Here, the key is set, encryption and decryption are done, and the matrix
//     is written to an outside file
// ==================================================================================================================================================


#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

#include "Hill.h"

// Function Prototypes


// == Class Function ================================================================================================================================


bool Hill::setKey(const string& key)
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





std::string Hill::encrypt(const string& plaintext)
{

    return "";
}





std::string Hill::decrypt(const string& ciphertext)
{

    return "";
}


// == end of Class Functions ========================================================================================================================