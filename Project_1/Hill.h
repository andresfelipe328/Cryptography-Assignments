// ==================================================================================================================================================
// Author: Andres F. Romero N.
// Begin Date: 03/24/21
// End Date: 03/31/21
// File Description: This is the class structure of the Hill class.
// ==================================================================================================================================================


#ifndef HILL_H
#define HILL_H

#include <vector>   /* For vectors */
#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */

#include "CipherInterface.h"

using namespace std;

/**
 * This class implements the playfair cipher.
 * The class extends the abstract class 
 * CipherInterface.
 */

class Hill: public CipherInterface
{
	// The public members 
	public:
		virtual bool setKey(const string& key);

		virtual string encrypt(const string& plaintext);

		virtual string decrypt(const string& ciphertext);

		//void SetMatrix(const char (&matrixTemp)[5][5]);
			
		//void PrintMatrix();
		
	// The protected members 
	protected:
		string CipherKey;
        string Ciphertext;
        string Plaintext;
		//char matrix[5][5];
};

#endif