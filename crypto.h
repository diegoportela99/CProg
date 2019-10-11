/*******************************************************************************
 * This header file enables other parts of the program to call the functions 
 * defined in crypto.c
*******************************************************************************/

#ifndef CRYPTOHEADER
#define CRYPTOHEADER

/* Encrypts 64 bit blocks of plaintext, given an array of plaintext blocks,
   the number of blocks of plaintext, and a key. Writes ciphertext to file. */
int encrypt(unsigned long long plaintext[], int number_of_blocks);

/* Decrypts a file into 64 bit blocks of plaintext, given a key. */
int decrypt(unsigned long long* plaintext_p);

/* Reads the header of an encrypted file, and gets the number of ciphertext
   blocks in the file */
int read_header();

#endif