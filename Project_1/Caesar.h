// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the class structure of the Caesar class.
// ==================================================================================================================================================


#ifndef CEASAR_H
#define CEASAR_H

#include <vector>   /* For vectors */
#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */

#include "CipherInterface.h"

using namespace std;

/**
 * This class implements the row transposition cipher.
 * The class extends the abstract class 
 * CipherInterface.
 */

class Caesar: public CipherInterface
{
	// The public members 
	public:
        //~RowTrans(){delete[] CipherKey;}

		virtual bool setKey(const string& key);

		virtual string encrypt(const string& plaintext);

		virtual string decrypt(const string& ciphertext);
		
	// The protected members 
	protected:
		int    CipherKey;
		string Plaintext;
		string Ciphertext;
};

#endif