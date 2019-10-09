
#ifndef CRYPTOHEADER
#define CRYPTOHEADER

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

/* Database name */
#define DBNAME "database"

/* Key file name*/
#define KEYNAME "key"

/* Source of random numbers */
#define URANDOM "/dev/urandom"

/* Defines a modulus which behaves correctly for negative values (unlike  "%").
Mathematically, negative inputs to mod should wrap round to positive. 
NOTE: Does not work for x < -y. */
#define MOD(x,y) ((((x)%(y))+(y))%(y)) 

/* Selects the rightmost 32 bits of a 64 bit int */
#define EIGHTLOWMASK 0xFFFFFFFF

/* The number of blocks of key material. */
#define NUMBER_OF_KEY_BLOCKS 4

/* The number of sections of key used for key whitening */
#define NUMBER_OF_WKEYS 4

/* The number of sections of key used in the F function */
#define NUMBER_OF_KEYS 32

/* The number of cycles of Wolfram's Rule 30 applied in the F function. Could
be as low as 1, but higher numbers cause greater non-linearity and diffusion. */
#define WOLFRAMCYCLES 16

/*******************************************************************************
 * Structure Definitions
*******************************************************************************/
/* This structure holds a 'cycle' of keys - all the keys needed to encrypt a
single block of ciphertext. In addition, it holds a pointer to the previous
key cycle - needed in decryption to iterate backwards through key cycles, but
also needed for deallocation of memory. */

struct key_cycle {
    /* The sections of key used for key whitening */
    unsigned long long wkey[NUMBER_OF_WKEYS]; 

    /* The sections of key used in the F function */
    unsigned long long key[NUMBER_OF_KEYS]; 
    
    /* Pointer to previous key cycle. Needed to iterate backwards through key
       cycles for decryption */
    struct key_cycle* previous_key_cycle_p; 
};
typedef struct key_cycle key_cycle_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/
/* Initializes the first key cycle */
key_cycle_t* init_key_cycle(unsigned long long key[NUMBER_OF_KEY_BLOCKS]);

/* Schedules a new key cycle given the previous one */
key_cycle_t* key_schedule(key_cycle_t* previous_key_cycle_p);

/* Frees a key cycle and all previous key cycles */
void free_key_cycles(key_cycle_t* key_cycle_p);

/* Saves the encrypted text to a file, with an integer header which is the
   number of blocks of ciphertext saved. */
int save_ciphertext(unsigned long long* ciphertext_p, int number_of_blocks);

/* Reads the header of an encrypted file, and gets the number of ciphertext
   blocks in the file */
int read_header();

/* Loads an encrypted file into memory */
unsigned long long* load_ciphertext(int number_of_blocks);

/* The core encryption scheme is a feistel network. It takes a block and key 
   material, and either encryts or decrypts the block */
unsigned long long feistel_network(unsigned long long plaintext,key_cycle_t key,
    int encrypt);

/* Also known as a "round function", the heart of a feistel cipher. Takes a 
   block of plaintext, performs pseudo-random number generation with
   it as the seed, and then XORs in the key. Is strongly non-linear. */
unsigned long f_function(unsigned long plaintext, unsigned long key);

/* Performs the pseudo-random number generation used in the F-function. */
unsigned long wolframs_rule_30(unsigned long plaintext);

/* Cryptographically Secure Pseudo-Random Number Generator. Generates a key 
   from system randomness provided in /dev/urandom. Is Linux system dependent.*/
unsigned long long csprng();

/* Saves the key to a file, or loads it from one. */
int file_handle_key(unsigned long long key[NUMBER_OF_KEY_BLOCKS],int save);

#ifdef GRAPHICALDEBUG
    /* Graphs Wolfram's rule 30 */
    void graph_rule_30(unsigned long row);
#endif

/* Ends the initial if, stopping errors if this header is included twice */
#endif
