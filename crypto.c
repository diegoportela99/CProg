/*******************************************************************************
 * WOLFRAM'S RULE-30 BASED BLOCK CIPHER
 * Name: Owen Dowley
 * Student ID: 13234505
 * 
 * Description:
 * This is an AES equivalent cipher, which uses 256 bit keys and encrypts 64
 * bit plaintext blocks. It operates on the Feistel network structure, 
 * repeatedly using a highly non-linear F function to fufill Shannon's principle
 * of confusion, and repeated XORing input with key material (known as "key 
 * whitening") to fufill Shannon's principle of diffusion.
 * 
 * The F-function in this cipher impliments a round of Wolfram's Rule 30 - a 
 * cellular automaton which has seen usage in pseudo-random number generation
 * because of the emergant semi-random pattern which occurs.
 * 
 * Note: it is worst practice to use your own cryptographic algorithms. Always
 * use standard algorithms - I recommend TwoFish, which is freely available.
 * Standard well-known algoritms have shown they can withstand modern 
 * cryptanalysis techniques - this code hasn't. This code is probably 
 * cryptographically secure, but achieves this by merit of shuffling plaintext 
 * much more repeatedly than a better designed cipher would need to. This 
 * results in very slow performance.
 * Additionally, this cipher is probably vulnerable to sidechannel attacks.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "block.h"
#include "debug.h"
#include "cryptohelper.h"

/*******************************************************************************
 * Function Prototypes - Other prototypes in cryptohelper.h
*******************************************************************************/

/* Decrypts a file into 64 bit blocks of plaintext, given a key. */
block_t* decrypt();

/*******************************************************************************
 * This function encrypts an array of blocks of plaintext, and saves the
 * resulting ciphertext to a file. This file has an integer header containing
 * the number of 64 bit blocks in the file.
 * inputs:
 * - plaintext[] | An array of plaintext to be encrypted.
 * - number_of_blocks | The number of blocks of plaintext, i.e., number of 
 *                    | elements in the plaintext array
 * - key[] | An array of key material used to encrypt the plaintext.
 * outputs:
 * - 1 or 0 | Has encrytion succeeded ? 1 : 0
*******************************************************************************/
int encrypt(block_t* block_p) {

    int i; /* iterator */
    unsigned long long key[NUMBER_OF_KEY_BLOCKS];
    int number_of_blocks = block_p->number_of_blocks;
    block_t* next_block_p;

    #ifdef DEBUG
        print_title("ENCRYTPING");
    #endif
    
    for(i=0;i<NUMBER_OF_KEY_BLOCKS;i++) {
        /* Get key blocks from Cryptographically Secure Pseudo-Random Number
        Generator */
        key[i] = csprng(); 
    }

    /* Save the key to file */
    file_handle_key(key,1);

    /* Initiate the key_cycle, storing the pointer to it */
    key_cycle_t* key_cycle_p = init_key_cycle(key);

    /* ciphertext_p is a pointer to memory to contain the ciphertext.
       MALLOC! Associated free() is later in encrypt */
    unsigned long long* ciphertext_p  = (unsigned long long*)malloc(sizeof
        (unsigned long long)*number_of_blocks);
    
    /* Check if malloc succeeded */
    if(ciphertext_p == NULL) {
        printf("Malloc failed for ciphertext_p\n");
        return(0);
    }

    /* Generate key cycles for each block of plaintext, and then encrypt them.*/
    for(i=0; i<number_of_blocks;i++) {
        key_cycle_p = key_schedule(key_cycle_p);
        #ifdef DETAILEDDEBUG
            print_header("Scheduling key:\n");
            printf("key_cycle_p = %p\n",key_cycle_p);
        #endif
        (*(ciphertext_p+i)) = feistel_network(block_p->number,*key_cycle_p,1);
        next_block_p = block_p->previous;
        /* FREE BLOCK_P */
        free(block_p);
        block_p = next_block_p;
    }

    save_ciphertext(ciphertext_p, number_of_blocks);
    free(ciphertext_p);
    ciphertext_p = NULL;

    free_key_cycles(key_cycle_p);

    return(1);
}

/*******************************************************************************
 * Decrypt
*******************************************************************************/
block_t* decrypt() {
    unsigned long long* plaintext_p;
    unsigned long long key[NUMBER_OF_KEY_BLOCKS];
    /* We need to hang onto the last generated key_cycle_p so we can hand it
    to free_key_cycles */
    key_cycle_t* last_key_cycle_p;
    int cipherBlocks,i;
    unsigned long long* ciphertext_p = NULL;\
    /* Used to return */
    block_t* block_p;

    #ifdef DEBUG
        print_title("DECRYTPING");
    #endif

    file_handle_key(key,0);

    cipherBlocks = read_header();
    if(!cipherBlocks) {
        printf("Loading ciphertext failed\n");
        return(0);
    }

    plaintext_p = (unsigned long long*)malloc(sizeof(unsigned long long)
        *cipherBlocks);

    ciphertext_p = load_ciphertext(cipherBlocks);
    
    key_cycle_t* key_cycle_p = init_key_cycle(key);
    
    for(i=0;i<cipherBlocks; i++) {
        key_cycle_p = key_schedule(key_cycle_p);
        #ifdef DETAILEDDEBUG
            print_header("Scheduling key:\n");
            printf("key_cycle_p = %p\n",key_cycle_p);
        #endif
    }

    last_key_cycle_p = key_cycle_p;

    for(i=cipherBlocks-1;i>=0;i--) {
        *(plaintext_p+i) = feistel_network(
            *(ciphertext_p+i),*key_cycle_p,0);  
        key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
    }

    #ifdef DEBUG
        print_header("Returning plaintext:\n");
        for(i=0;i<cipherBlocks;i++) {
            printf("%llx\n",*(plaintext_p+i));
        }
    #endif

    for(i=0;i<cipherBlocks;i++) {
        block_p = add_ull_to_block(block_p,*(plaintext_p+i));
    }
    free(plaintext_p);
    free(ciphertext_p);
    ciphertext_p = NULL;
    free_key_cycles(last_key_cycle_p);
    
    return(block_p);
}