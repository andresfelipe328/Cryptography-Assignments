// ==================================================================================================================================================
// Author: Andres F. Romero N
// Date Finished: 04/30/21
// File Name: AES.cpp
// Language: C++
// Description: This file contains the implementation of the AES class, which contains a setkey, encrypt, and decrypt functions. In addition, two
// 		more functions, twoCharToHexByte and charToHex, were added as helper functions for setKey function. The primary goal of this file is to set
//		the cipher key and enc/decrypt blocks of text and send them back to the driver file.
//
// ==================================================================================================================================================


#include "AES.h"

// == setKey ========================================================================================================================================

bool AES::setKey(const unsigned char* keyArray)
{
    // Variables
    int size = 32;
	int keyIndex = 0;
	int aesKeyIndex = 0;
    char code = keyArray[0];
	
    while (aesKeyIndex != 16)
	{
		/* Convert the key if the character is valid */
		if ((this->aes_key[aesKeyIndex] = twoCharToHexByte(keyArray + keyIndex)) == 'z')
			return false;

		/* Go to the second pair of characters */
		keyIndex += 2;

		/* Increment the index */
		++aesKeyIndex;
	}
	
	/* Print the key */
    std::cout << "AES KEY: ";
	for (keyIndex = 0; keyIndex < 16; ++keyIndex)
		fprintf(stdout, "%x", this->aes_key[keyIndex]);
	fprintf(stdout, "\n");

    return true;
}

// == end of setKey =================================================================================================================================





// == encrypt =======================================================================================================================================

unsigned char* AES::encrypt(const unsigned char* plainText)
{
	// Variable
    int blockSize = 17;
    unsigned char enc_out[blockSize];
	unsigned char dec_out[17];
    unsigned char* 	bytes = new unsigned char[17];
	memset(enc_out, 0, 17);

	// Seting encryption key
	AES_KEY enc;
	AES_set_encrypt_key(aes_key, 128, &enc);

    // Encrypt
	AES_ecb_encrypt(plainText, enc_out, &enc, AES_ENCRYPT);

	for (int i = 0; i < blockSize; ++i)
		bytes[i] = enc_out[i];

    return bytes;
}

// == end of encrypt ================================================================================================================================





// == decrypt =======================================================================================================================================

unsigned char* AES::decrypt(const unsigned char* cipherText)
{
	// Variable
    int blockSize = 17;
    unsigned char dec_out[17];
	unsigned char* 	bytes = new unsigned char[17];
	memset(dec_out, 0, 17);

	// Seting decryption key
	AES_KEY dec;
	AES_set_decrypt_key(aes_key, 128, &dec);

	// Decrypt
    AES_ecb_encrypt(cipherText, dec_out, &dec, AES_DECRYPT);

	for (int i = 0; i < blockSize; ++i)
		bytes[i] = dec_out[i];
	
    return bytes;
}

// == end of decrypts ===============================================================================================================================





// == twoCharTwoHexByte =============================================================================================================================

unsigned char AES::twoCharToHexByte(const unsigned char* twoChars)
{
	/* The byte */
	unsigned char singleByte;

	/* The second character */
	unsigned char secondChar;

	/* Convert the first character */
	if ((singleByte = charToHex(twoChars[0])) == 'z')
	{
		/* Invalid digit */
		return 'z';
	}

	/* Move the newly inserted nibble from the
	 * lower to upper nibble.
	 */
	singleByte = (singleByte << 4);

	/* Conver the second character */
	if ((secondChar = charToHex(twoChars[1])) == 'z')
		return 'z';

	/* Insert the second value into the lower nibble */
	singleByte |= secondChar;

	return singleByte;
}

// == end of twoCharTwoHexByte ======================================================================================================================





// == charToHex =====================================================================================================================================

unsigned char AES::charToHex(const char& character)
{
	/* Is the first digit 0-9 ? */
	if (character >= '0' && character <= '9')
		/* Convert the character to hex */
		return character - '0';
	/* It the first digit a letter 'a' - 'f'? */
	else if (character >= 'a' && character <= 'f')
		/* Conver the cgaracter to hex */
		return (character - 97) + 10;
	/* Invalid character */
	else return 'z';
}

// == end of charToHex ==============================================================================================================================